#include "../imaging/Image.hpp"
#include "../Utilities/Utilities.hpp"
#include <iostream>


#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkXMLImageDataWriter.h>
typedef std::vector<Image> ImageVector;

namespace Test{
    class GenericTest {
        public:
            GenericTest(ImageVector images_, bool full_test_) : full_test(full_test_), images(images_) {};

            virtual void test() = 0;

            void write(const std::string folder) {
                volume = folder;
                const int start_idx = full_test ? 1 : noisy_start_idx;
                const int end_idx = full_test ? 100 : noisy_end_idx;
                for(int i = start_idx; i < end_idx; i++){
                    std::string out = "processed_medical_images/" + folder +  "/cthead-8bit0" + std::to_string(i) + ".jpg";
                    images[i - 1].write(Tools::getChar(out), ImageType::JPG);
                }
            }
            void buildVolume(){
                vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();

                // Set dimensions and spacing of the image data
                
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

                volume = (volume == "") ? "original_volume" : volume;
                // Create VTK writer and set file name
                vtkSmartPointer<vtkXMLImageDataWriter> writer = vtkSmartPointer<vtkXMLImageDataWriter>::New();
                writer->SetFileName(Tools::getChar(volume + ".vti"));
                writer->SetInputData(imageData);

                // Write the image data to file
                writer->Write();
            }


        protected:
            std::string volume = "";
            bool full_test;
            ImageVector images;
    };



    class GaussTest : public GenericTest{
        public:
            GaussTest(ImageVector images_, bool full_test_) :  GenericTest(images_, full_test_) {};

            void test() {
                const double start = omp_get_wtime();
                #pragma omp parallel for schedule(dynamic, 2)
                for(int i = 1; i < 100; i++){
                    images[i - 1].fft_convolve(gauss_five_size, gauss_five_size, gauss_std_five, 7, 7);
                    
                    std::cout << "Convolved "<< i <<  std::endl;    
                }
                const double end = omp_get_wtime();
                std::cout << "Time taken by fft convolution: "<< end - start << std::endl;
                
            }

            
    };

    class SobelTest : public GenericTest{
        public:
            SobelTest(ImageVector images_, bool full_test_) :  GenericTest(images_, full_test_) {};

            void test() {
                const int start_idx = full_test ? 1 : noisy_start_idx;
                const int end_idx = full_test ? 100 : noisy_end_idx;
                const double start = omp_get_wtime();
                #pragma omp parallel for schedule(dynamic, 2)
                for(int i = start_idx; i < end_idx; i++){
                    for(uint8_t c = 0; c < channel_num; c++){
                        images[i - 1] = images[i - 1].sobel();
                    }
                    std::cout << "Sobel "<< i <<  std::endl;    
                }
                const double end = omp_get_wtime();
                std::cout << "Time taken by sobel: "<< end - start << std::endl;
                
            } 
    };

    class KuwaharaTest : public GenericTest{
        public:
            KuwaharaTest(ImageVector images_, bool full_test_) : GenericTest(images_, full_test_) {};

            void test() {
                const int start_idx = full_test ? 1 : noisy_start_idx;
                const int end_idx = full_test ? 100 : noisy_end_idx;
                const double start = omp_get_wtime();
                for(int i = start_idx; i < end_idx; i++){
                    images[i - 1].kuwahara();
                }
                const double end = omp_get_wtime();
                std::cout << "Time taken by kuwahara: "<< end - start << std::endl; 
            } 
    };

}
