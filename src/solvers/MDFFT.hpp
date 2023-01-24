#include <complex>
#include <iostream>
#include <vector>
#include <cmath>
#include <valarray>
#include <omp.h>

#include "../types/ComplexMatrix.hpp"

typedef std::complex<double> Complex;
typedef std::valarray<Complex> ComplexArray;

namespace Solver{

    /**
     * 2D FFT solver. Perform twice the following: computes FFT on each row, insert in col major the transformed rows(avoid transpose).
    */
    class MDFFT{

        public:
            /**
             * @brief Construct a new MDFFT object, storing input matrix and assuming first a forward FFT will be performed.
             * 
             * @param input_data_ the data on which the transform will be performed.
             */
            MDFFT(const ComplexMatrix &input_data_):input_data(input_data_), output_data(input_data_), eval_inverse(false) { }

            /**
             * @brief Compute iteratively the FFT on 2D input
             * 
             * @return ComplexMatrix the input in fourier domain
             */
            ComplexMatrix solveIterative();

            /**
             * @brief Get the Inverse of the performed FFT
             * 
             * @return ComplexMatrix the result
             */
            ComplexMatrix getInverse();


        private:
            ComplexMatrix input_data;
            ComplexMatrix output_data;

            //void transpose(ComplexMatrix in);

            bool eval_inverse;
    };
}

