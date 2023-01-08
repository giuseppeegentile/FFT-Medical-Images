#include "FFT.hpp"
#include "CSV_Reader.hpp"
#include "Image.hpp"
#include "Ctz.hpp"
#include "Filtering_traits.hpp"


int main() {
    using Solver::MDFFT;
    using Solver::FFT;
    using Solver::Ctz;

    omp_set_nested(1);
    omp_set_num_threads(4);

    ComplexArray input;
    Utility::File       file("../csv/sus.csv");
    Utility::CSV_Reader reader(file);
    const int size = reader.getArraySize();
    input = reader.getArray();


    ComplexArray i_n;
    Utility::File       f_n("../csv/temp_test.csv");
    Utility::CSV_Reader re(f_n);
    const int temp_size = re.getArraySize();
    i_n = re.getArray();
    Solver::Ctz solver(i_n);
    ComplexArray o = solver.solve(false);

    Utility::OutputFile of;
    of.open("../csv/A.csv");
    for(size_t i = 0; i < o.size(); i++){
        of << o[i].real() << "," << o[i].imag() << "\n";
    }
    of.close();
    //Image test("../medical_images/stanford/cthead-8bit075.jpg");
    //Image ref_image("../medical_images/stanford/cthead-8bit075.jpg");
    

    omp_set_nested(1);
    //double start = omp_get_wtime();
    for(int i = 1; i < 99; i++){
        std::string fn = "../medical_images/stanford/cthead-8bit0" + std::to_string(i) + ".jpg";
        const int length = fn.length();
        char* c = new char[length + 1]; //+1 fot the \n
        strcpy(c, fn.c_str());
        Image test_med(c);
        #pragma omp parallel for
        for(int i = 0; i < 3; i++){
            test_med.fft_convolve(i, gauss_five_size, gauss_five_size, gauss_std_five, 7, 7);
        }
        std::string out = "../processed_medical_images/cthead-8bit0" + std::to_string(i) + ".jpg";
        
        char* c_out = new char[length + 1]; //+1 fot the \n
        strcpy(c_out, out.c_str());
        test_med.write(c_out, ImageType::JPG);
    }
    
    
    
    //double end = omp_get_wtime();

    //std::cout << "Time taken by fft convolution: "<< end - start << std::endl;
    //ref_image.diff(test);
    //ref_image.write("../processed_medical_images/diff.jpg", ImageType::JPG);
    //test.write("../processed_medical_images/out_blur_fft.jpg", ImageType::JPG);

    std::cout << "_________________________________________________________________________________________" << std::endl;

    return 0;
}


