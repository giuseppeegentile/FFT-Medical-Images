#include "../imaging/Image.hpp"
#include "../Utilities/Utilities.hpp"
#include <iostream>

typedef std::vector<Image> ImageVector;

namespace Test{

    class GenericTest {
        public:
            GenericTest(ImageVector images_) : images(images_) {};

            virtual void test() = 0;

            void write(const std::string folder) {
                for(int i = 1; i < 100; i++){
                    std::string out = "../processed_medical_images/" + folder +  "/cthead-8bit0" + std::to_string(i) + ".jpg";
                    images[i - 1].write(Tools::getChar(out), ImageType::JPG);
                }
            }

        protected:
            ImageVector images;

            void testFunction(void (Image::* function)()){
                
            }
    };


    class GaussTest : public GenericTest{
        public:
            GaussTest(ImageVector images_) : GenericTest(images_) {};

            void test() {
                double start = omp_get_wtime();
                #pragma omp parallel for schedule(dynamic, 2)
                for(int i = 1; i < 100; i++){
                    for(uint8_t c = 0; c < channel_num; c++){
                        images[i - 1] = images[i - 1].fft_convolve(c, gauss_five_size, gauss_five_size, gauss_std_five, 7, 7);
                    }
                    std::cout << "Convolved "<< i <<  std::endl;    
                }
                double end = omp_get_wtime();
                std::cout << "Time taken by fft convolution: "<< end - start << std::endl;
                
            }
    };

    class SobelTest : public GenericTest{
        public:
            SobelTest(ImageVector images_) : GenericTest(images_) {};

            void test() {
                double start = omp_get_wtime();
                #pragma omp parallel for schedule(dynamic, 2)
                for(int i = 1; i < 100; i++){
                    for(uint8_t c = 0; c < channel_num; c++){
                        images[i - 1] = images[i - 1].sobel();
                    }
                    std::cout << "Sobel "<< i <<  std::endl;    
                }
                double end = omp_get_wtime();
                std::cout << "Time taken by sobel: "<< end - start << std::endl;
                
            }
            
    };
}
