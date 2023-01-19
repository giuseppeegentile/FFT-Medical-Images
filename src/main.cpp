#include "Test/test.hpp"
#include "solvers/FFT.hpp"
#include "Utilities/CSV_Reader.hpp"
#include "traits/Parallel_configuration.hpp"


#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkXMLImageDataWriter.h>

int main() {
    using Solver::FFT;
    using namespace Test;
    omp_set_nested(nesting);
    omp_set_num_threads(n_threads);



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

    vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();

    // Set dimensions and spacing of the image data
    int dimensions[3] = {images[0].getHeight(), images[0].getWidth(), images.size()};
    imageData->SetDimensions(dimensions);
    imageData->AllocateScalars(VTK_UNSIGNED_CHAR, channel_num);
    imageData->SetSpacing(1.0, 1.0, 1.0);

    for (size_t i = 0; i < images.size(); i++) {
        for (int j = 0; j < images[i].getHeight(); j++) {
            for (int k = 0; k < images[i].getWidth(); k++) {
                for (int c = 0; c < images[i].getChannels(); c++) {
                    // get the pixel value
                    const uint8_t pixelValue = images[i].getData()[(j * images[i].getWidth() + k) * channel_num + c];
                    // set the pixel value in the vtk image
                    imageData->SetScalarComponentFromDouble(j, k, i, c, pixelValue);
                }
            }
        }
    }
    
    // Create VTK writer and set file name
    vtkSmartPointer<vtkXMLImageDataWriter> writer = vtkSmartPointer<vtkXMLImageDataWriter>::New();
    writer->SetFileName("volume.vti");
    writer->SetInputData(imageData);

    // Write the image data to file
    writer->Write();



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

/*
    KuwaharaTest* ku_test = new KuwaharaTest(ku, false);
    ku_test->test();
    ku_test->write("kuwhara");
    std::cout << "_________________________________________________________________________________________" << std::endl;
*/



    return 0;
}

