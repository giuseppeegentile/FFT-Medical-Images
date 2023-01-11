#include "FFT.hpp"
#include "CSV_Reader.hpp"
#include "Image.hpp"
#include "Ctz.hpp"
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

    //show difference of anisotropic filtering
/*
    Image diff_ref("../medical_images/stanford/cthead-8bit075.jpg");
    diff_ref.diff(ref_image);
    diff_ref.write("../src/diff_ani.jpg", ImageType::JPG);*/

    omp_set_nested(1);
    //double start = omp_get_wtime();
    
    for(int i = 0; i < 100; i++){
        /*if(i == 75){
            Image test_sob(c);
            test_sob.sobel();
            test_sob.write("../src/sob.jpg", ImageType::JPG);
        }*/
        #pragma omp parallel for schedule(static)
        for(uint8_t c = 0; c < channel_num; c++){
            images[i] = images[i].fft_convolve(c, gauss_five_size, gauss_five_size, gauss_std_five, 7, 7);
        }
    }
    //double end = omp_get_wtime();
    //std::cout << "Time taken by fft convolution: "<< end - start << std::endl;
    for(int i = 1; i < 100; i++){
        std::string out = "../processed_medical_images/cthead-8bit0" + std::to_string(i) + ".jpg";
        images[i].write(Tools::getChar(out), ImageType::JPG);
    }

    Image test("../medical_images/stanford/cthead-8bit075.jpg");
    Image ref_image("../medical_images/stanford/cthead-8bit075.jpg");
    
    test.anisotropic_diffusion(ref_image,300, 0.7/*, 200*/);
    ref_image.write("../src/ani.jpg", ImageType::JPG);

    
    //ref_image.diff(test);
    //ref_image.write("../processed_medical_images/diff.jpg", ImageType::JPG);
    //test.write("../processed_medical_images/out_blur_fft.jpg", ImageType::JPG);

    std::cout << "_________________________________________________________________________________________" << std::endl;









    return 0;
}

