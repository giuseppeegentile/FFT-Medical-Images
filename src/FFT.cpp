#ifndef HH_SOLVERFFFT_HH
#define HH_READCVSHEADER_HH

#include "FFT.hpp"
namespace Solver{
    using namespace Solver;

    ComplexArray
    FFT::recursiveAlgorithm(const  ComplexArray& input){
        const size_t N = input.size();
        if (N <= 1) return input;

        ComplexArray even = input[std::slice(0, N/2, 2)];
        ComplexArray odd = input[std::slice(1, N/2, 2)];

        ComplexArray y_even = recursiveAlgorithm(even);
        ComplexArray y_odd = recursiveAlgorithm(odd);
        ComplexArray ret(N);

        for (size_t k = 0; k < N/2; ++k) {
            Complex omega = std::exp(Complex(0, -2 * M_PI * k / N))* y_odd[k];
            ret[k] = y_even[k] + omega;
            ret[k+N/2] = y_even[k] - omega;
        }
        return ret;
    }

    void FFT::solveRecursive(){
        output_data = recursiveAlgorithm(input_data);
    }

    void FFT::solveIterative(const bool parallel = true){
        const size_t N = input_data.size();
        const int log = std::log2(N);
        omp_set_num_threads(4);
        omp_set_nested(1);
        ComplexArray temp =  eval_inverse ? output_data : input_data;
        #pragma omp task shared(output_data, input_data) firstprivate(N) if(parallel)
        #pragma omp parallel for schedule(static) if(parallel)
        for (unsigned int i = 0; i < N; i++) {
            output_data[bitReverse(i, std::log2(N))] = temp[i];
        }
        for(int s = 1; s <= log; s++){ //bottom up in the tree, start from the root ( = 1)
            int m = 1 << s;
            int half_m = m >> 1;
            #pragma omp parallel for shared(output_data) schedule(static) if(parallel)
            for(size_t k = 0; k < N; k += m){
                for(int j = 0; j < half_m; j++){
                    Complex omega = std::exp(Complex(0, -2 * M_PI * j /***/ /*inv_coeff*/ / m));
                    Complex temp1 = omega * output_data[k + j + half_m];
                    Complex temp2 = output_data[k + j];
                    output_data[k + j] = temp2 + temp1;
                    output_data[k + j + half_m] = temp2 - temp1;
                }
            }
        }
    }



    const int FFT::bitReverse(unsigned int x, const int log2n) const {
        int n = 0;
        for (int i = 0; i < log2n; i++) {
            n <<= 1;
            n |= (x & 1);
            x >>= 1;
        }
        return n;
    }

    ComplexArray FFT::getOutput(){
        return output_data;
    }

    ComplexArray FFT::getInverse(){
        eval_inverse = true;
        const double n = output_data.size();
        output_data = output_data.apply(std::conj);
        solveIterative(false);
        output_data = output_data.apply(std::conj);
        output_data /= n;
        eval_inverse = false; //back to original value
        return output_data;
    }
}

#endif // !HH_SOLVERFFFT_HH

