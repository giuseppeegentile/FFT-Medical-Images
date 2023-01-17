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
                const int start_idx = full_test ? 1 : noisy_start_idx;
                const int end_idx = full_test ? 100 : noisy_end_idx;
                for(int i = start_idx; i < end_idx; i++){
                    std::string out = "../processed_medical_images/" + folder +  "/cthead-8bit0" + std::to_string(i) + ".jpg";
                    images[i - 1].write(Tools::getChar(out), ImageType::JPG);
                }
            }

        protected:
            bool full_test = true;
            ImageVector images;
    };


    class GaussTest : public GenericTest{
        public:
            GaussTest(ImageVector images_) : GenericTest(images_) {};

            void test() {
                const double start = omp_get_wtime();
                #pragma omp parallel for schedule(dynamic, 2)
                for(int i = 1; i < 100; i++){
                    images[i - 1] = images[i - 1].fft_convolve(gauss_five_size, gauss_five_size, gauss_std_five, 7, 7);
                    
                    std::cout << "Convolved "<< i <<  std::endl;    
                }
                const double end = omp_get_wtime();
                std::cout << "Time taken by fft convolution: "<< end - start << std::endl;
                
            }
    };

    class SobelTest : public GenericTest{
        public:
            SobelTest(ImageVector images_) : GenericTest(images_) {};

            void test() {
                full_test = false;
                const double start = omp_get_wtime();
                #pragma omp parallel for schedule(dynamic, 2)
                for(int i = noisy_start_idx; i < noisy_end_idx; i++){
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
            KuwaharaTest(ImageVector images_) : GenericTest(images_), outputs(100, Image(medical_img_size + kuwahara_pad, medical_img_size + kuwahara_pad, channel_num)) {};

            void test() {
                ImageVector outputs(100, Image(medical_img_size + kuwahara_pad, medical_img_size + kuwahara_pad, channel_num));
                const double start = omp_get_wtime();
                #pragma omp parallel for schedule(dynamic, 2)
                for(int i = noisy_start_idx; i < noisy_end_idx; i++){
                    images[i - 1].pad_for_kuwahara(outputs[i - 1]);
                    outputs[i - 1].kuwahara();
                    outputs[i - 1].crop_to_center(medical_img_size, medical_img_size, outputs[i - 1]);
                    std::cout << "Kuwahara "<< i <<  std::endl;    
                }
                const double end = omp_get_wtime();
                std::cout << "Time taken by kuwahara: "<< end - start << std::endl; 
            } 

             void write(const std::string folder) {
                const int start_idx = full_test ? 1 : noisy_start_idx;
                const int end_idx = full_test ? 100 : noisy_end_idx;
                for(int i = start_idx; i < end_idx; i++){
                    std::string out = "../processed_medical_images/" + folder +  "/cthead-8bit0" + std::to_string(i) + ".jpg";
                    outputs[i - 1].write(Tools::getChar(out), ImageType::JPG);
                }
                Image tmp(images[75]);
                tmp = tmp.diff(outputs[75]);
                tmp.write("../src/diff.jpg",ImageType::JPG);
                

            }
        private:
            ImageVector outputs;
    };

}
