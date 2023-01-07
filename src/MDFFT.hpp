#include <complex>
#include <iostream>
#include <vector>
#include <cmath>
#include <valarray>
#include <omp.h>

#include "ComplexMatrix.hpp"

typedef std::complex<double> Complex;
typedef std::valarray<Complex> ComplexArray;

namespace Solver{

    class MDFFT{

        public:
            MDFFT(const ComplexMatrix &input_data_):input_data(input_data_), output_data(input_data_), eval_inverse(false) { }

            ComplexMatrix solveIterative();

            ComplexMatrix getInverse();


        private:
            ComplexMatrix input_data;
            ComplexMatrix output_data;

            //void transpose(ComplexMatrix in);

            bool eval_inverse;
    };
}

