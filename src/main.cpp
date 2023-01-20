#include "Test/test.hpp"
#include "solvers/FFT.hpp"
#include "Utilities/CSV_Reader.hpp"
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

    
    /*GenericTest* gauss_test = new GaussTest(gaussed_images, true);
    gauss_test->test();
    gauss_test->write("gaussed");
    gauss_test->buildVolume();*/


 
    /*GenericTest* sobel_test = new SobelTest(sob, true);
    sobel_test->test();
    sobel_test->write("sobel");
    sobel_test->buildVolume();*/



    GenericTest* ku_test = new KuwaharaTest(ku, true);
    ku_test->test();
    ku_test->write("kuwahara");
    ku_test->buildVolume();
    std::cout << "_________________________________________________________________________________________" << std::endl;




    return 0;
}

