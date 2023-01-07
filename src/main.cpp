#include "FFT.hpp"
#include "CSV_Reader.hpp"
#include "Image.hpp"
#include "Ctz.hpp"

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

    Image test("../images/tigre.jpg");
    Image test2("../images/tigre.jpg");
    Image test_not_fft("../images/tigre.jpg");
    size_t s = 4;
    const size_t lenght = s * s;
    double *gauss = new double[lenght];
    gauss[0] = 1/16.; gauss[1] = 2/16.; gauss[2] = 1/16.; gauss[3] = 2/16.;
    gauss[4] = 1/16.; gauss[5] = 2/16.; gauss[6] = 1/16.; gauss[7] = 2/16.;
    gauss[8] = 1/16.; gauss[9] = 2/16.; gauss[10] = 1/16.; gauss[11] = 2/16.;
    gauss[12] = 1/16.; gauss[13] = 2/16.; gauss[14] = 1/16.; gauss[15] = 2/16.;


    double start = omp_get_wtime();

    omp_set_nested(1);
    #pragma omp parallel for
    for(int i = 0; i < 3; i++){
        test.fft_convolve(i, s, s, gauss, 2, 2);
    }
    double end = omp_get_wtime();
    std::cout << "Time taken by fft convolution: "<< end - start << std::endl;
    test2.diff(test);
    test2.write("../images/diff.jpg", ImageType::JPG);
    test.write("../images/out_blur_fft.jpg", ImageType::JPG);

    start = omp_get_wtime();
    #pragma omp parallel for
    for(int i = 0; i < 3; i++){
        test_not_fft.convolve(i, s, s, gauss, 2, 2);
    }
    end = omp_get_wtime();
    std::cout << "Time taken by normal convolution: " << end -start << std::endl;
    delete[] gauss;


    Image fft_non_fft("../images/out_blur_fft.jpg");
    fft_non_fft.diff(test_not_fft);
    fft_non_fft.write("../images/Diff_fft_convs.jpg", ImageType::JPG);
    test_not_fft.write("../images/out_blur_non_fft.jpg", ImageType::JPG);

    std::cout << "_________________________________________________________________________________________" << std::endl;


    Image img1("../images/img1.jpg");
    Image img2("../images/img2.jpg");

    #pragma omp parallel for 
    for(int c  = 0; c < 3; c++)
        img1.imageProduct(img2, c);
    img1.write("../images/squared.jpg", ImageType::JPG);




    return 0;
}


