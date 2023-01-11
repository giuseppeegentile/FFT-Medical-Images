#include "MDFFT.hpp"
#include "FFT.hpp"


namespace Solver{
    using namespace Solver;
    ComplexMatrix MDFFT::solveIterative(){
        size_t col = input_data.getWidth();
        size_t row = input_data.getHeight();

        omp_set_num_threads(4);
        omp_set_nested(1);
        //FFT on matrix rows
        #pragma omp parallel for schedule(dynamic, 2) 
        for(size_t r = 0; r < row; r++){
            ComplexArray arr_row(col);
            arr_row = input_data.getRow(r);
            FFT fft_iter(arr_row);
            fft_iter.solveIterative(false);
            output_data.addColMajor(r, fft_iter.getOutput());
        }
        // Computing transpose of the matrix
        //output_data = output_data.transpose();
        //again FFT on matrix rows
        #pragma omp parallel for  schedule(dynamic, 2) 
        for(size_t r = 0; r < row; r++){
            ComplexArray arr_row(col);
            arr_row = output_data.getRow(r);
            FFT fft_iter(arr_row);
            fft_iter.solveIterative(false);
            output_data.addColMajor(r, fft_iter.getOutput());
        }
        //again transpose
        //output_data = output_data.transpose();
        return output_data;
    }

 

    ComplexMatrix MDFFT::getInverse(){
        size_t col = output_data.getWidth();
        size_t row = output_data.getHeight();

        omp_set_num_threads(4);
        omp_set_nested(1);
        //iFFT on matrix rows
        #pragma omp parallel for schedule(dynamic, 2) 
        for(size_t r = 0; r < row; r++){
            ComplexArray arr_row(col);
            arr_row = output_data.getRow(r);
            FFT fft_iter(arr_row);
            input_data.addColMajor(r, fft_iter.getInverse());
        }
          // Computing transpose of the matrix
        
        //input_data = input_data.transpose();
        //again iFFT on matrix rows
        #pragma omp parallel for  schedule(dynamic, 2) 
        for(size_t r = 0; r < row; r++){
            ComplexArray arr_row(col);
            arr_row = input_data.getRow(r);
            FFT fft_iter(arr_row);
            output_data.add(r, fft_iter.getInverse());
        }
        //again transpose
        output_data = output_data.transpose();
        return output_data;
    }
}