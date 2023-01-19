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

class Image {
    public:        
        Image(const char* filename, int channel_force = 3) {
            if(read(filename, channel_force)) {
                std::cout << "Reading correctly the image " << filename << std::endl;
                size = w * h * channels;
            }else{
                std::cout << "ERROR: Incorrect reading the Image" <<filename << std::endl;
            }
        }

        Image(int w_, int h_, int channels_) : w(w_), h(h_), channels(channels_){
            size = w * h * channels;
            data = std::make_unique<uint8_t[]>(size);
        }

        Image(const Image& img) : w(img.getWidth()), h(img.getHeight()), channels(img.getChannels()), size(img.getSize()), data(std::make_unique<uint8_t[]>(img.getSize())) {
            std::copy_n(img.data.get(), img.getSize(), data.get());
        }

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


        bool read(const char* filename, int channel_force = 0);
        bool write(const char* filename, ImageType type);
        
        inline const size_t getSize() const {return size; };
        

        inline const  int getWidth() const  {return w;} ;
        inline const int getHeight() const {return h;};
        inline const uint8_t getChannels() const {return channels;};

        Image& convolve(uint8_t channel, uint32_t kernel_width, uint32_t kernel_height, double kernel[], uint32_t center_row, uint32_t center_col);
        Image& fft_convolve(size_t ker_w, size_t ker_h, const double ker[], uint32_t center_row, uint32_t center_col);

        const void padKernel(size_t ker_width, size_t ker_height, const double ker[], uint32_t center_row, uint32_t center_col, size_t pad_width, size_t pad_height, ComplexMatrix &pad_ker) const ;
        Image& diff(Image& img);

        const Image& sobel();

        
        const void anisotropic_diffusion(Image& dst, int num_iterations, float k) const;

        void merge_2d(const std::vector<Image> &images);

        Image& ctz_convolve(const uint8_t channel, size_t ker_w, size_t ker_h, const double ker[], uint32_t center_row, uint32_t center_col);

        const void pad_for_kuwahara(const Image& res) const;

        void kuwahara();

        void crop_to_center(const int width, const int height, const Image& res);
    private:
        int w;
        int h;
        uint8_t channels;
        size_t size;
        std::unique_ptr<uint8_t[]> data;
};