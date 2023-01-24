#include <complex>
#include <cstdio>
#include <stdint.h>
#include <omp.h>
#include <iostream>
#include <string.h>
#include <numeric>

#include "../traits/Imaging_Traits.hpp"
#include "../library/stb_image.h"
#include "../library/stb_image_write.h"
#include "../types/ComplexMatrix.hpp"
#include "../traits/Filtering_traits.hpp"
#include "../solvers/Ctz.hpp"
#include "../solvers/MDFFT.hpp"
#include <memory>


/**
 * @brief Storage and various methods on images.
 * 
 */
class Image {
    public:        
        /**
         * @brief Construct a new Image object
         * 
         * @param filename the path on the filesystem of the image 
         * @param channel_force how many channels the image has
         */
        Image(const char* filename, int channel_force = 3) {
            if(read(filename, channel_force)) {
                std::cout << "Reading correctly the image " << filename << std::endl;
                size = w * h * channels;
            }else{
                std::cout << "ERROR: Incorrect reading the Image" <<filename << std::endl;
            }
        }

        /**
         * @brief Construct a new Image object, given the width, heigth, and channels
         * 
         * @param w_ image pixel width
         * @param h_ image pixel heigth
         * @param channels_ image channels (i.e. RGB = 3 channels, greyscale = 2)
         */
        Image(int w_, int h_, int channels_) : w(w_), h(h_), channels(channels_){
            size = w * h * channels;
            data = std::make_unique<uint8_t[]>(size);
        }

        /**
         * @brief Copy constructor
         * 
         * @param img the image to shallow-copy
         */
        Image(const Image& img) : w(img.getWidth()), h(img.getHeight()), channels(img.getChannels()), size(img.getSize()), data(std::make_unique<uint8_t[]>(img.getSize())) {
            std::copy_n(img.data.get(), img.getSize(), data.get());
        }

        /**
         * @brief Assignment operator override
         * 
         * @param other the image assigned to this
         * @return Image& 
         */
        Image& operator=(const Image& other) {
            if (this == &other) return *this;
            data = std::make_unique<uint8_t[]>(other.getSize());
            w = other.getWidth();
            h = other.getHeight();
            channels = other.getChannels();
            size = other.getSize();
            std::copy_n(other.data.get(), other.getSize(), data.get());
            return *this;
        }

        /*~Image() {
            if (data) {
                stbi_image_free(data.get());
            }
        }*/


        /**
         * @brief Read image from file
         * 
         * @param filename where the image is stored
         * @param channel_force the channels of the image
         * @return true if read is successful
         */
        bool read(const char* filename, int channel_force = 0);
        
        /**
         * @brief Write image to file
         * 
         * @param filename  where the image will be stored
         * @param type the image extension (i.e. JPG)
         * @return true if write was successful
         */
        bool write(const char* filename, ImageType type);
        
        /**
         * @brief Get the size of the image on disk
         * 
         * @return const size_t calculated as width * heigth * channels
         */
        inline const size_t getSize() const {return size; };
        
        /**
         * @brief Get the Width of the image
         * 
         * @return const int 
         */
        inline const  int getWidth() const  {return w;};

        /**
         * @brief Get the Heigth of the image
         * 
         * @return const int 
         */
        inline const int getHeight() const {return h;};

        /**
         * @brief Get the Channels of the image
         * 
         * @return const int 
         */
        inline const uint8_t getChannels() const {return channels;};


        /**
         * @brief Convolution without using the FFT algorithm. Perform the naive convolution operator with no optimization.
         * 
         * @param channel the channel where to perform the convolution
         * @param kernel_width width of the kernel
         * @param kernel_height heigth of the kernel
         * @param kernel the matrix of the kernel
         * @param center_row the center row of the kernel, up where to which row to pad the kernel to image next
         * @param center_col the center column of the kernel, up where to which column to pad the kernel to image next
         * @return Image& the convolved image
         */
        Image& convolve(uint8_t channel, uint32_t kernel_width, uint32_t kernel_height, double kernel[], uint32_t center_row, uint32_t center_col);
        /**
         * @brief Convolution using the FFT algorithm. Parallel on the channels, and uses the parallel version of the 2D FFT
         * @param ker_w width of the kernel
         * @param ker_h heigth of the kernel
         * @param ker  the matrix of the kernel
         * @param center_row the center row of the kernel, up where to which row to pad the kernel to image next
         * @param center_col the center column of the kernel, up where to which column to pad the kernel to image next
         * @return Image& the convolved image
         */
        Image& fft_convolve(size_t ker_w, size_t ker_h, const double ker[], uint32_t center_row, uint32_t center_col);


        /**
         * @brief Pad the kernel to the image. Puts the first "center_row" (and col) on the top left of the image. The remaining are put on the other borders: top-right, bottom-left, bottom-right.
         * 
         * @param ker_width width of the kernel
         * @param ker_height heigth of the kernel
         * @param ker the matrix of the kernel
         * @param center_row the center row of the kernel, up where to which row to pad the kernel to image next
         * @param center_col the center column of the kernel, up where to which column to pad the kernel to image next
         * @param pad_width the width of the image
         * @param pad_height the heigth of the image
         * @param pad_ker the kernel padded
         */
        const void padKernel(size_t ker_width, size_t ker_height, const double ker[], uint32_t center_row, uint32_t center_col, size_t pad_width, 
                                                                                                                        size_t pad_height, ComplexMatrix &pad_ker) const ;

        /**
         * @brief Perform the pixel by pixel difference of two images (this and img).
         * 
         * @param img the image to differentiate from this
         * @return * Image& the difference image
         */
        Image& diff(Image& img);

        /**
         * @brief Perform the sobel operator. Create image emphasising edges, using a 3x3 gauss transformation.
         * 
         * @return const Image& the filtered image
         */
        const Image& sobel();

        /**
         * @brief Perona-Malik diffusion. Uses the sobel operator implemented (and therefore a 3x3 gauss). 
         * 
         * @param dst the output image
         * @param num_iterations the number of iteration that will be performed to differentiate the image.
         * @param k the reduction factor, must be in range (0, 1). Higher numbers will reduce more noisy images, but more image information will be lost. 
         */
        const void anisotropic_diffusion(Image& dst, int num_iterations, float k) const;

        //void merge_2d(const std::vector<Image> &images);

        /**
         * @brief Convolution using the Bluestein algorithm. Works for non power of two images width or heigth (or both)
         * @param ker_w width of the kernel
         * @param ker_h heigth of the kernel
         * @param ker  the matrix of the kernel
         * @param center_row the center row of the kernel, up where to which row to pad the kernel to image next
         * @param center_col the center column of the kernel, up where to which column to pad the kernel to image next
         * @return Image& the convolved image
         */
        Image& ctz_convolve(const uint8_t channel, size_t ker_w, size_t ker_h, const double ker[], uint32_t center_row, uint32_t center_col);


        //const void pad_for_kuwahara(const Image& res) const;

        /**
         * @brief Apply the kuwahara filter, parallel over the channels.
         * 
         */
        void kuwahara();


        //void crop_to_center(const int width, const int height, const Image& res);

        /**
         * @brief Get the storage data of the image
         * 
         * @return uint8_t* 
         */
        inline uint8_t* getData() { return data.get(); }
    private:
        /**
         * @brief Width of the image
         * 
         */
        int w;
        /**
         * @brief Heigth of the image
         * 
         */
        int h;
        /**
         * @brief Channels of the image
         * 
         */
        uint8_t channels;
        /**
         * @brief size of the Storage image ( = w * h * channels)
         * 
         */
        size_t size;
        /**
         * @brief Storage of the image
         * 
         */
        std::unique_ptr<uint8_t[]> data;
};