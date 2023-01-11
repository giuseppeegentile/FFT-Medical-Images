#include "FFT.hpp"
#include "CSV_Reader.hpp"
#include "Image.hpp"
#include "Parallel_configuration.hpp"
#include "Utilities.hpp"

int main() {
    using Solver::MDFFT;
    using Solver::FFT;
    using Solver::Ctz;
    omp_set_nested(1);
    omp_set_num_threads(n_threads);


    std::vector<Image> images;
    //read and populate images vector
    for(int i = 1; i < 100; i++){
        std::string s = "../medical_images/stanford/cthead-8bit0" + std::to_string(i) + ".jpg";
        Image img(Tools::getChar(s), 2);
        images.push_back(img);
    }

    Image merge(images[0].getWidth(), 100 * images[0].getWidth(), images[0].getChannels());
    merge.merge_2d(images);
    merge.write("../src/att.jpg", ImageType::JPG);
    
    for(uint8_t c = 0; c < channel_num; c++){
        //images[i] = images[i].fft_convolve(c, gauss_five_size, gauss_five_size, gauss_std_five, 7, 7);
        merge.ctz_convolve(c, gauss_five_size, gauss_five_size, gauss_std_five, 7, 7);
    }
    
    


    return 0;
}

