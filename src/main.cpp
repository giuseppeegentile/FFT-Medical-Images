#include "solvers/FFT.hpp"
#include "Utilities/CSV_Reader.hpp"
#include "imaging/Image.hpp"
#include "traits/Parallel_configuration.hpp"
#include "Utilities/Utilities.hpp"

int main() {
    using Solver::FFT;
    omp_set_nested(nesting);
    omp_set_num_threads(n_threads);



    std::vector<Image> images;
    //read and populate images vector
    for(int i = 1; i < 100; i++){
        std::string s = "../medical_images/stanford/cthead-8bit0" + std::to_string(i) + ".jpg";
        Image img(Tools::getChar(s), 2);
        images.push_back(img);
    }


    omp_set_nested(1);
    double start = omp_get_wtime();
    //#pragma omp parallel for schedule(dynamic, 2)
    for(int i = 1; i < 5; i++){
        #pragma omp parallel for schedule(dynamic, 2)
        for(uint8_t c = 0; c < channel_num; c++){
            images[i - 1] = images[i - 1].fft_convolve(c, gauss_five_size, gauss_five_size, gauss_std_five, 7, 7);
        }
        std::cout << "Convolved "<< i <<  std::endl;    
    }
    double end = omp_get_wtime();
    std::cout << "Time taken by fft convolution: "<< end - start << std::endl;
    for(int i = 1; i < 100; i++){
        std::string out = "../processed_medical_images/gaussed/cthead-8bit0" + std::to_string(i) + ".jpg";
        images[i - 1].write(Tools::getChar(out), ImageType::JPG);
    }

    //Image test("../medical_images/stanford/cthead-8bit075.jpg");
    //Image ref_image("../medical_images/stanford/cthead-8bit075.jpg");
    

    
    //ref_image.diff(test);
    //ref_image.write("../processed_medical_images/diff.jpg", ImageType::JPG);
    //test.write("../processed_medical_images/out_blur_fft.jpg", ImageType::JPG);

    std::cout << "_________________________________________________________________________________________" << std::endl;




    return 0;
}

