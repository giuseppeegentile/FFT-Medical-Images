
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


    std::vector<Image> tmp;
    tmp.reserve(4);
    for(int j = 1; j <= (1 + 3); j++){
        std::string s = "../medical_images/stanford/cthead-8bit0" + std::to_string(j) + ".jpg";
        Image img(Tools::getChar(s), 2);
        tmp.push_back(img);
    }
    Image merged(medical_img_size * 2,medical_img_size * 2, channel_num);
    merged.merge_2d(tmp);
    images.push_back(merged);

    double s = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, 2)
    for(uint8_t c = 0; c < channel_num; c++){
        images[0] = images[0].fft_convolve(c, gauss_five_size, gauss_five_size, gauss_std_five, 7, 7);
    }double e = omp_get_wtime();
    std::cout << e-s << std::endl;

    std::string out = "../processed_medical_images/gaussed/cthead-8bit0" + std::to_string(1000) + ".jpg";
    images[0].write(Tools::getChar(out), ImageType::JPG);
        


    return 0;
}

