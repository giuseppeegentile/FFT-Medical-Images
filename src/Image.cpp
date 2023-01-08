#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "Image.hpp"

Image::Image(const char* filename, int channel_force) {
    if(read(filename, channel_force)) {
        std::cout << "Reading correctly the image " << filename << std::endl;
        size = w * h * channels;
    }else{
        std::cout << "ERROR: Incorrect reading the Image" <<filename << std::endl;
    }
}

Image::Image(int w_, int h_, int channels_) : w(w_), h(h_), channels(channels_){
    size = w * h * channels;
    data = new uint8_t[size];
}
Image::~Image() {
    stbi_image_free(data);
}

bool Image::read(const char* filename, int channel_force) {
	data = stbi_load(filename, &w, &h, &channels, channel_force);
	channels = channel_force == 0 ? channels : channel_force;
	return data != NULL;
}

bool Image::write(const char* filename, ImageType type) {
	int success;
    switch (type) {
        case ImageType::PNG:
        success = stbi_write_png(filename, w, h, channels, data, w*channels);
        break;
        case ImageType::BMP:
        success = stbi_write_bmp(filename, w, h, channels, data);
        break;
        case ImageType::JPG:
        success = stbi_write_jpg(filename, w, h, channels, data, 100);
        break;
        case ImageType::TGA:
        success = stbi_write_tga(filename, w, h, channels, data);
        break;
    }
    if(success != 0) {
        std::cout << "SUCCESS: wrote correctly " << filename << std::endl;
        return true;
    }
    else {
        std::cout << "FAILED: while writing " << filename << std::endl;
        return false;
    }
}

Image& Image::convolve(uint8_t channel, uint32_t kernel_width, uint32_t kernel_height, double kernel[], uint32_t center_row, uint32_t center_col){
    const size_t upd_size = w * h;
    uint8_t *updated = new uint8_t[upd_size];
    uint64_t center_index = center_row * kernel_width + center_col; //row major
    for(uint64_t i = channel; i  < size; i += channels){ //loop on pixels
        double sum = 0;
        for(int j = -((int)center_row); j < (int)(kernel_height - center_row); ++j){//loop on kernel
            long row = ((long)(i / channels)) / w - j;
            if(row < 0 || row > h - 1) continue; //out of bound
            for(int k = -((int)center_col); k < (int)(kernel_width - center_col); ++k){
                long col = ((long)(i / channels) % w) - k;
                if(col < 0 || col > w - 1) continue; //out of bound
                //at this point, if I didn't go on out of bound, I have to do the dot product on the image pixel
                sum += kernel[center_index + j * (long)kernel_width + k] * data[(row * w + col) * channels + channel];
            }
        }
        updated[i / channels] = (uint8_t)(std::round(sum) < 0.0 ? 0 : (std::round(sum) > 255.0 ? 255 : std::round(sum)));
    }
    for(uint64_t i = channel; i < size; i += channels){
        data[i] = updated[i / channels];
    }
    delete[] updated;
    
    return *this;
}


void Image::padKernel(size_t ker_width, size_t ker_height, const double ker[], uint32_t center_row, uint32_t center_col, size_t pad_width, size_t pad_height, ComplexMatrix &pad_ker) {
	//padded so center of kernel is at top left
	for(long i = -((long)center_row); i < (long)ker_height - center_row; ++i) {
		uint32_t r = (i<0) ? i + pad_height : i;
		for(long j = -((long)center_col); j<(long)ker_width - center_col; ++j) {
			uint32_t c = (j < 0) ? j + pad_width : j;
            pad_ker.add(r, c, std::complex<double>(ker[(i + center_row) * ker_width + (j + center_col)], 0));
		}
	}
}


Image& Image::fft_convolve(uint8_t channel, size_t ker_w, size_t ker_h, const double ker[], uint32_t center_row, uint32_t center_col) {
    using Solver::MDFFT;
	//pad image
	ComplexMatrix pad_img(w, h);
	for(int i = 0; i < h; ++i) {
		for(int j = 0; j < w; ++j) {
            pad_img.add(i, j, std::complex<double>(data[(i * w + j)* channels + channel], 0));
		}
	}

	//pad kernel
	ComplexMatrix pad_ker(w, h);
	padKernel(ker_w, ker_h, ker, center_row, center_col, w, h, pad_ker);
    //padKernel_second_version(ker_w, ker_h, ker, center_row, center_col, pw, ph, pad_ker);

	//convolution

    MDFFT fft_img(pad_img);
    pad_img = fft_img.solveIterative();
    MDFFT fft_ker(pad_ker);
    pad_ker = fft_ker.solveIterative();
    ComplexMatrix dot(w, h);
    pad_img.dotProduct(pad_ker, dot);
    MDFFT inv(dot);
    dot = inv.getInverse();


	//update pixel data
	for(int i = 0; i < h; ++i) {
		for(int j = 0; j < w; ++j) {
			data[(i * w + j) * channels + channel] = (uint8_t)(round(dot(i, j).real()) < 0 ? 0 : (round(dot(i, j).real()) > 255 ? 255 : round(dot(i, j).real())));
            //data[(i*w+j)*channels+channel] = (uint8_t)(std::round(dot(i, j).real())); //versione psichedelica
		}
	}

	return *this;
}



void Image::diff(Image& img) {
    int min_channels = channels < img.channels ? channels : img.channels;
    int min_width = w < img.w ? w : img.w;
    int min_height = h < img.h ? h : img.h;
    
    for(int i = 0; i < min_height; i++){
        for(int j = 0; j < min_width; j++){
            for(int c = 0; c < min_channels; c++){
                uint8_t diff = (uint8_t)std::abs(data[(i * w + j) * channels + c] - img.data[(i * img.w + j) * img.channels + c]);
                data[(i * w + j) * channels + c] = (uint8_t)(round(diff) < 0 ? 0 : (diff) > 255 ? 255 : round(diff));
            }
        }
    }

}

#endif
#endif