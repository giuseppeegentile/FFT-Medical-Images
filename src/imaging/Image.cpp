#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "Image.hpp"


bool Image::read(const char* filename, int channel_force) {
    int ch = (int)channels;
	size = w * h * ch;
    data.reset(stbi_load(filename, &w, &h, &ch, channel_force));
	channels = channel_force == 0 ? channels : channel_force;
	return data != NULL;
}

bool Image::write(const char* filename, ImageType type) {
	int success;
    switch (type) {
        case ImageType::PNG:
        success = stbi_write_png(filename, w, h, channels, data.get(), w*channels);
        break;
        case ImageType::BMP:
        success = stbi_write_bmp(filename, w, h, channels, data.get());
        break;
        case ImageType::JPG:
        success = stbi_write_jpg(filename, w, h, channels, data.get(), 100);
        break;
        case ImageType::TGA:
        success = stbi_write_tga(filename, w, h, channels, data.get());
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
        updated[i / channels] = convert_to_pixel(sum);
    }
    for(uint64_t i = channel; i < size; i += channels){
        data[i] = updated[i / channels];
    }
    delete[] updated;
    
    return *this;
}


const void Image::padKernel(size_t ker_width, size_t ker_height, const double ker[], uint32_t center_row, uint32_t center_col, size_t pad_width, size_t pad_height, ComplexMatrix &pad_ker) const {
	//padded so center of kernel is at top left
	for(long i = -((long)center_row); i < (long)ker_height - center_row; ++i) {
		uint32_t r = (i<0) ? i + pad_height : i;
		for(long j = -((long)center_col); j<(long)ker_width - center_col; ++j) {
			uint32_t c = (j < 0) ? j + pad_width : j;
            pad_ker.add(r, c, std::complex<double>(ker[(i + center_row) * ker_width + (j + center_col)], 0));
            //pad_ker[r * w + c] = Complex(ker[(i + center_row) * ker_width + (j + center_col)], 0);

		}
	}
}

Image& Image::fft_convolve(size_t ker_w, size_t ker_h, const double ker[], uint32_t center_row, uint32_t center_col) {
    using Solver::MDFFT;
    for(int channel = 0; channel < channels; channel++){
        //pad image
        ComplexMatrix pad_img(w, h);
        for(int i = 0; i < h; ++i) {
            for(int j = 0; j < w; ++j) {
                //pad_img[(i * w + j)] = (Complex(data[(i * w + j) * channels + channel], 0));
                pad_img.add(i, j, Complex(data[(i * w + j) * channels + channel], 0));
            }
        }
        //pad kernel
        ComplexMatrix pad_ker(w, h);
        padKernel(ker_w, ker_h, ker, center_row, center_col, w, h, pad_ker);

        //convolution
        MDFFT fft_img(pad_img);
        pad_img = fft_img.solveIterative();
        
        MDFFT fft_ker(pad_ker);
        pad_ker = fft_ker.solveIterative();

        ComplexMatrix dot(w, h);
        pad_img.dotProduct(pad_ker, dot);

        MDFFT inv(dot);
        ComplexMatrix out = inv.getInverse();


        //update pixel data
        for(int i = 0; i < h; ++i) {
            for(int j = 0; j < w; ++j) {
                data[(i * w + j) * channels + channel] = convert_to_pixel(out(i, j).real());
                //data[(i*w+j)*channels+channel] = (uint8_t)(std::round(dot(i, j).real())); //versione psichedelica
            }
        }
    }

	return *this;
}



Image& Image::diff(Image& img) {
    int min_channels = channels < img.channels ? channels : img.channels;
    int min_width = w < img.w ? w : img.w;
    int min_height = h < img.h ? h : img.h;
    
    for(int i = 0; i < min_height; i++){
        for(int j = 0; j < min_width; j++){
            for(int c = 0; c < min_channels; c++){
                uint8_t diff = (uint8_t)std::abs(data[(i * w + j) * channels + c] - img.data[(i * img.w + j) * img.channels + c]);
                data[(i * w + j) * channels + c] = convert_to_pixel(diff);
            }
        }
    }
    return (*this);
}

const Image& Image::sobel(){
    Image cpy_x(*this);
    Image cpy_y(*this);

    /*Image gx(getWidth(), getHeight(), getChannels());
    Image gy(getWidth(), getHeight(), getChannels());*/
    cpy_x = cpy_x.fft_convolve(3, 3, sobel_kernel_x, 1, 1);
    cpy_y = cpy_y.fft_convolve(3, 3, sobel_kernel_y, 1, 1);

/*
    gx.write("../src/try_x.jpg", ImageType::JPG);
    gy.write("../src/try_y.jpg", ImageType::JPG);*/
    //compute the gradient
    for(size_t i = 0; i < size; i++)
        data[i] = std::hypot(cpy_x.data[i], cpy_y.data[i]) / 8.0;

    return *this;

}

//Perona - Malik equation to calculate the new intensity of each pixels
//k: sensitivity of the filter
const void Image::anisotropic_diffusion(Image& dst, int num_iterations, float k) const {
    Image g_norm(*this);
    
    for(int ch = 0; ch <= getChannels(); ch++){
        g_norm.sobel();
        Image newDest(dst);
        for (int i = 0; i < num_iterations; i++) {
            for (int y = 1; y < getWidth() - 1; y++) {
                for (int x = 1; x < getHeight() - 1; x++) {
                    uint8_t c = data[(y * getWidth() + x) * getChannels() + ch];
                    uint8_t c_north = data[((y - 1) * getWidth() + x) * getChannels() + ch];
                    uint8_t c_south = data[((y + 1) * getWidth() + x) * getChannels() + ch];
                    uint8_t c_west = data[(y * getWidth() + x - 1) * getChannels() + ch];
                    uint8_t c_east = data[(y * getWidth() + x + 1) * getChannels() + ch];

                    uint8_t grad_north = std::exp(-std::pow(g_norm.data[((y - 1) * getWidth() + x) * getChannels() + ch] / k, 2));
                    uint8_t grad_south = std::exp(-std::pow(g_norm.data[((y + 1) * getWidth() + x) * getChannels() + ch]/ k, 2));
                    uint8_t grad_west = std::exp(-std::pow(g_norm.data[(y * getWidth() + x - 1) * getChannels() + ch] / k, 2));
                    uint8_t grad_east = std::exp(-std::pow(g_norm.data[(y * getWidth() + x + 1) * getChannels() + ch] / k, 2));

                    newDest.data[(y * getWidth() + x) * getChannels() + ch] = c + 0.25 * 
                                                                                (grad_north * (c_north - c) + 
                                                                                grad_south * (c_south - c) + 
                                                                                grad_west * (c_west - c) + 
                                                                                grad_east * (c_east - c));
                }
            }   
            dst = newDest;         
        }
    }

}

void Image::merge_2d(const std::vector<Image> &images) {
    const int img_size = images.size();
    for(int img = 0; img < img_size / 2; img++){
        for(int i = 0; i < medical_img_size; i++){
            for(int j = 0; j < medical_img_size; j++){
                for(uint8_t c = 0; c < getChannels(); c++){
                    data[((i* getWidth() + (img * medical_img_size) + j) * channels + c)] = images[img].data[(i * medical_img_size + j) * channels + c];
                }
            }
        }
    }
    const int offset = getWidth() * getHeight() *getChannels() / 2;
    for(int img = img_size / 2, k = 1; img < img_size; img++, k++){
        for(int i = 0; i < medical_img_size; i++){
            for(int j = 0; j < medical_img_size; j++){
                for(uint8_t c = 0; c < getChannels(); c++){
                    data[((i* getWidth() + (img * medical_img_size) + j) * channels + c) + offset] = images[img].data[(i * medical_img_size + j) * channels + c];
                }
            }
        }
    }
/*
    for(int i = 0; i < medical_img_size; i++){
        for(int j = 0; j < medical_img_size; j++){
            for(uint8_t c = 0; c < getChannels(); c++){
                data[((i* getWidth() + j) * channels + c) ] = images[0].data[(i * medical_img_size + j) * channels + c];
            }
        }
    }

    for(int i = 0; i < medical_img_size; i++){
        for(int j = 0; j < medical_img_size; j++){
            for(uint8_t c = 0; c < getChannels(); c++){
                data[((i* getWidth() + medical_img_size + j) * channels + c)] = images[1].data[(i * medical_img_size + j) * channels + c];
            }
        }
    }

*/
}


Image& Image::ctz_convolve(const uint8_t channel, size_t ker_w, size_t ker_h, const double ker[], uint32_t center_row, uint32_t center_col) {
    using Solver::Ctz;
	//pad image
	MyComplexArray pad_img(w * h);
    double start = omp_get_wtime();
	for(int i = 0; i < h; ++i) {
		for(int j = 0; j < w; ++j) {
            //pad_img.add(i, j, std::complex<double>(data[(i * w + j)* channels + channel], 0));
            pad_img[i * getWidth() + j] = Complex(data[(i * w + j)* channels + channel], 0);
		}
	}
    double end = omp_get_wtime();
    std::cout << end - start << std::endl;

	//pad kernel
	/*ComplexMatrix pad_ker(w, h);
	padKernel(ker_w, ker_h, ker, center_row, center_col, w, h, pad_ker);*/

	//convolution
    start = omp_get_wtime();
    Ctz fft_img(pad_img);
    pad_img = fft_img.solve(false);
    end = omp_get_wtime();
    std::cout << end - start << std::endl;

	//update pixel data
    /*
	for(int i = 0; i < h; ++i) {
		for(int j = 0; j < w; ++j) {
			data[(i * w + j) * channels + channel] = (uint8_t)(round(dot(i, j).real()) < 0 ? 0 : (round(dot(i, j).real()) > 255 ? 255 : round(dot(i, j).real())));
            
		}
	}
*/
	return *this;
}

const void Image::pad_for_kuwahara(const Image& res) const{
    for(int ch = 0; ch < channel_num; ch++){
        for(int i = 0; i < kuwahara_pad / 2; i++){
            for(int j = 0; j < res.getWidth(); j++){
                res.data[(i * res.getWidth() + j)*channels + ch] = 0.;
            }
        }
        for(int i = kuwahara_pad / 2; i < medical_img_size + kuwahara_pad / 2; i++){
            for(int j = 0; j < kuwahara_pad / 2; j++){
                res.data[(i * res.getWidth() + j)*channels + ch] = 0.;
            }
            for(int j = medical_img_size + kuwahara_pad / 2; j < medical_img_size + kuwahara_pad; j++){
                res.data[(i * res.getWidth() + j)*channels + ch] = 0.;
            }
        }
        for(int i = medical_img_size + kuwahara_pad / 2; i < res.getHeight(); i++) {
            for(int j = 0; j < res.getWidth(); j++){
                res.data[(i * res.getWidth() + j)*channels + ch] = 0.;
            }
        }
        for(int i = kuwahara_pad / 2; i < medical_img_size + kuwahara_pad / 2; i++){
            for(int j = kuwahara_pad / 2; j < medical_img_size + kuwahara_pad / 2; j++){
                res.data[(i * res.getWidth() + j)*channels + ch] = data[((i - kuwahara_pad / 2) * getWidth() + j - kuwahara_pad / 2)*channels + ch];
            }
        }
    }
}

//-fast-math, try with it
void Image::kuwahara()  {
    for(int ch = 0; ch < channels; ch++){
        //i want the min standard dev, so take the biggest possible value at the starting point
        double min_avg = 255.;
        double min_dev = std::numeric_limits<double>::max();
        int min_row = 0;
        int min_col = 0;

        for(int row = 0; row < getHeight(); row += kuwahara_kernel_size){ 
            for (int col = 0; col < getWidth(); col += kuwahara_kernel_size){
                double sum = 0.0;
                //for every row in the divided image of kernel size, sum the row
                for(int i = row; i < row + kuwahara_kernel_size; i++){
                    for(int j = col; j < col + kuwahara_kernel_size; j++){
                        sum += data[(i * getWidth() + j) * channels + ch];
                    }
                }
                const double avg = sum / (double)(kuwahara_kernel_size * kuwahara_kernel_size);
                double dev = 0.0;                
                for(int i = row; i < row + kuwahara_kernel_size; i++){
                    for(int j = col; j < col + kuwahara_kernel_size; j++){
                        const double tmp =(data[(i * getWidth() + j)*channels  + ch ] - avg) ;
                        dev += tmp * tmp;
                    }    
                }
                dev /= (kuwahara_kernel_size * kuwahara_kernel_size);
                dev = std::sqrt(dev);
                if(dev < min_dev){
                    min_dev = dev;
                    min_avg = avg;
                    min_row = row;
                    min_col = col;
                }
            }
        }

        for(int i = min_row + kuwahara_kernel_size/2; i < min_row + kuwahara_kernel_size; i++){
            for(int j = min_col + kuwahara_kernel_size/2; j < min_col + kuwahara_kernel_size; j++){
                data[(i * getWidth() + j) * channels + ch] = convert_to_pixel(min_avg);
            }
        }
    }
}


void Image::crop_to_center(const int width, const int height, const Image& res){
    for(int ch = 0; ch < channel_num; ch++){
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                res.data[(i * width + j) * channels + ch] = data[((i + kuwahara_kernel_size / 2) * getWidth() + j+ kuwahara_kernel_size / 2) * channels + ch];
            }
        }
    }
    
}


#endif
#endif

