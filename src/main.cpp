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
        std::string s = prefix + std::to_string(i) + format;
        Image img(Tools::getChar(s), 2);
        images.push_back(img);
    }
    std::vector<Image> gaussed_images(images);
    std::vector<Image> ku(images);
    std::vector<Image> sob(images);
    std::vector<Image> aniso(images);

  /*  
    GenericTest* gauss_test = new GaussTest(gaussed_images);
    gauss_test->test();
    gauss_test->write("gaussed");


    GenericTest* sobel_test = new SobelTest(sob);
    sobel_test->test();
    sobel_test->write("sobel");
*/

    /*Image res(images[75]);
    Image d(images[75]);
    images[75].pad_for_kuwahara(res);
    res.kuwahara();
    res.crop_to_center(medical_img_size, medical_img_size, res);
    res.write("../src/r.jpg", ImageType::JPG);
    
    d.diff(res);
    d.write("../src/diff.jpg", ImageType::JPG);*/


    KuwaharaTest* ku_test = new KuwaharaTest(ku, false);
    ku_test->test();
    ku_test->write("kuwhara");
    std::cout << "_________________________________________________________________________________________" << std::endl;




    return 0;
}

