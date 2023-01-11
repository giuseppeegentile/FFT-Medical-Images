#ifndef HH_SOLVERFFFT_HH
#define HH_SOLVERFFFT_HH

#include "FFT_Traits.hpp"


namespace Solver{

    class FFT{

        public:
            FFT(ComplexArray &input_data_):input_data(input_data_), output_data(input_data_), eval_inverse(false) {}

            void
            solveIterative(const bool parallel);

            void
            solveRecursive();

            ComplexArray
            getOutput();

            ComplexArray
            getInverse();

        private:
            //array storing the input
            ComplexArray input_data;
            //array storing the calculated output
            ComplexArray output_data;

            bool eval_inverse;

            ComplexArray
            recursiveAlgorithm(const  ComplexArray& input);

            const int 
            bitReverse(unsigned int x, const int log2n) const;

    };
}
#endif