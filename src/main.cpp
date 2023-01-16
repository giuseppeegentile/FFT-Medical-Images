#include "Test/test.hpp"
#include "solvers/FFT.hpp"
#include "Utilities/CSV_Reader.hpp"
#include "traits/Parallel_configuration.hpp"


int main() {
    using Solver::FFT;
    using namespace Test;
    omp_set_nested(nesting);
    omp_set_num_threads(n_threads);



    std::vector<Image> images;
    
    //read and populate images vector
    for(int i = 1; i < 100; i++){
        std::string s = "../medical_images/stanford/cthead-8bit0" + std::to_string(i) + ".jpg";
        Image img(Tools::getChar(s), 2);
        images.push_back(img);
    }
    std::vector<Image> gaussed_images(images);
    std::vector<Image> ku(images);
    std::vector<Image> sob(images);
    std::vector<Image> aniso(images);

    
    GenericTest* gauss_test = new GaussTest(gaussed_images);
    gauss_test->test();
    gauss_test->write("gaussed");


    GenericTest* sobel_test = new SobelTest(sob);
    sobel_test->test();
    sobel_test->write("sobel");


    std::cout << "_________________________________________________________________________________________" << std::endl;




    return 0;
}

