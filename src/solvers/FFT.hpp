#ifndef HH_SOLVERFFFT_HH
#define HH_SOLVERFFFT_HH

#include "../traits/FFT_Traits.hpp"

namespace Solver{

    class FFT{

        public:
            FFT(MyComplexArray &input_data_):input_data(input_data_), output_data(input_data_), eval_inverse(false) {}

            void
            solveIterative(const bool parallel);

            MyComplexArray
            getOutput();

            MyComplexArray
            getInverse();

        private:
            //array storing the input
            MyComplexArray input_data;
            //array storing the calculated output
            MyComplexArray output_data;

            bool eval_inverse;

            const int 
            bitReverse(unsigned int x, const int log2n) const;

    };
}
#endif