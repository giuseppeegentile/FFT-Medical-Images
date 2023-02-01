#include "Test/test.hpp"
#include "solvers/FFT.hpp"
#include "traits/Parallel_configuration.hpp"
#include <filesystem>



int main() {
    using Solver::FFT;
    using namespace Test;
    omp_set_nested(nesting);
    omp_set_num_threads(n_threads);

    std::filesystem::create_directories("processed_medical_images");
    std::filesystem::create_directories("processed_medical_images/gaussed");
    std::filesystem::create_directories("processed_medical_images/kuwahara");
    std::filesystem::create_directories("processed_medical_images/sobel");
    std::filesystem::create_directories("processed_medical_images/anisotropic");
    std::filesystem::create_directories("processed_medical_images/gaussed_difference");
    std::filesystem::create_directories("processed_medical_images/kuwahara_difference");
    std::filesystem::create_directories("processed_medical_images/sobel_difference");
    std::filesystem::create_directories("processed_medical_images/anisotropic_difference");

    std::vector<Image> images;
    
    //read and populate images vector
    for(int i = 1; i < 100; i++){
        std::string s = prefix + std::to_string(i) + format;
        Image img(Tools::getChar(s), channel_num);
        images.push_back(img);
    }
    std::vector<Image> gaussed_images(images);
    std::vector<Image> ku(images);
    std::vector<Image> sob(images);
    std::vector<Image> aniso(images);
    std::vector<Image> tmp(images);

    GenericTest* original_volume = new GaussTest(tmp, true);
    original_volume->buildVolume();


    std::cout << "_________________________________________________________________________________________" << std::endl;
    GenericTest* gauss_test = new GaussTest(gaussed_images, true);
    gauss_test->test();
    gauss_test->write("gaussed");
    gauss_test->writeDifference();
    gauss_test->buildVolume();


    std::cout << "_________________________________________________________________________________________" << std::endl;
    GenericTest* sobel_test = new SobelTest(sob, true);
    sobel_test->test();
    sobel_test->write("sobel");
    sobel_test->writeDifference();
    sobel_test->buildVolume();


    std::cout << "_________________________________________________________________________________________" << std::endl;
    GenericTest* ku_test = new KuwaharaTest(ku, true);
    ku_test->test();
    ku_test->write("kuwahara");
    ku_test->writeDifference();
    ku_test->buildVolume();
    std::cout << "_________________________________________________________________________________________" << std::endl;

    GenericTest* anisotropic_test = new AnisotropicTest(aniso, true);
    anisotropic_test->test();
    anisotropic_test->write("anisotropic");
    anisotropic_test->writeDifference();
    anisotropic_test->buildVolume();


    return 0;
}

