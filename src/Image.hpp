#include <complex>
#include <cstdio>
#include <stdint.h>
#include <omp.h>
#include <iostream>
#include <string.h>

#include "stb_image.h"
#include "stb_image_write.h"
#include "ComplexMatrix.hpp"
#include "MDFFT.hpp"
#include "CSV_Reader.hpp"
#include "Filtering_traits.hpp"
#include "Ctz.hpp"
typedef std::complex<double> Complex;
typedef std::valarray<Complex> ComplexArray;

enum class ImageType {
    PNG, JPG, BMP, TGA
};
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
            data = new uint8_t[size];
        }

        Image& operator=(const Image& other)  {
            if (this == &other) return *this;
    
            size = other.getSize();
            data = new uint8_t[size];
            w = other.getWidth();
            h = other.getHeight(); 
            channels = other.getChannels();
            
            std::copy(other.getData(), other.getData() + other.getSize(), data);
            return *this;
        }

        Image(const Image& img)  {
            *this = img;
        }

        ~Image() {
            delete[] data;
            data = NULL;
        }


        bool read(const char* filename, int channel_force = 0);
        bool write(const char* filename, ImageType type);
        
        inline const size_t getSize() const {return size; };
        inline uint8_t* getData() const {return data;};

        inline const  int getWidth() const  {return w;} ;
        inline const int getHeight() const {return h;};
        inline const uint8_t getChannels() const {return channels;};

        Image& convolve(uint8_t channel, uint32_t kernel_width, uint32_t kernel_height, double kernel[], uint32_t center_row, uint32_t center_col);
        Image& fft_convolve(uint8_t channel, size_t ker_w, size_t ker_h, const double ker[], uint32_t center_row, uint32_t center_col);

        const void padKernel(size_t ker_width, size_t ker_height, const double ker[], uint32_t center_row, uint32_t center_col, size_t pad_width, size_t pad_height, ComplexMatrix &pad_ker) const ;
        void diff(Image& img);

        const Image& sobel();

        //const void anisotropic_diffusion(Image& dst, int num_iterations, float k) const;
        const void anisotropic_diffusion(const Image& dest, const double dt, const int lambda, const int interations) const ;
        const void anisotropic_diffusion(Image& dst, int num_iterations, float k) const;

        void merge_2d(const std::vector<Image> &images);


        Image& ctz_convolve(uint8_t channel, size_t ker_w, size_t ker_h, const double ker[], uint32_t center_row, uint32_t center_col);
    private:
        size_t size;
        int w;
        int h;
        uint8_t *data;
        uint8_t channels;
};