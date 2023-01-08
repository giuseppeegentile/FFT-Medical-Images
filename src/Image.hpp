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
typedef std::complex<double> Complex;
typedef std::valarray<Complex> ComplexArray;

enum class ImageType {
    PNG, JPG, BMP, TGA
};
class Image {

    public:
        
        Image(const char* filename, int channel_force = 0);

        Image(int w_, int h_, int channels_ = 3);

        ~Image();

        bool read(const char* filename, int channel_force = 0);
        bool write(const char* filename, ImageType type);
        
        inline size_t getSize() {return size; };
        inline uint8_t* getData() {return data;};

        inline int getWidth() {return w;};
        inline int getHeight() {return h;};
        inline int getChannels() {return channels;};

        Image& convolve(uint8_t channel, uint32_t kernel_width, uint32_t kernel_height, double kernel[], uint32_t center_row, uint32_t center_col);
        Image& fft_convolve(uint8_t channel, size_t ker_w, size_t ker_h, const double ker[], uint32_t center_row, uint32_t center_col);

        void padKernel(size_t ker_width, size_t ker_height, const double ker[], uint32_t center_row, uint32_t center_col, size_t pad_width, size_t pad_height, ComplexMatrix &pad_ker);
        void diff(Image& img);

    private:
        size_t size;
        int w;
        int h;
        uint8_t *data;
        int channels;
};