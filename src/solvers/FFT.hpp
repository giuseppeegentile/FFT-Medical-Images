#ifndef HH_SOLVERFFFT_HH
#define HH_SOLVERFFFT_HH

#include "../traits/FFT_Traits.hpp"

namespace Solver{

    /**
     * 1D FFT solver. Uses the Cooley Tuckey algorithm with O(NlogN) complexity.
    */
    class FFT{

        public:
            /**
             * @brief Construct a new FFT object, storing input arrays and assuming first a forward FFT will be performed.
             * 
             * @param input_data_ the data on which the transform will be performed.
             */
            FFT(const MyComplexArray &input_data_):input_data(input_data_), output_data(input_data_), eval_inverse(false) {}

            /**
             * @brief Compute iteratively the FFT, using Cooley Tuckey
             * 
             * @param parallel true if the parallel version want to be used.
             */
            void
            solveIterative(const bool parallel);

            /**
             * @brief Returned the Output of the FFT
             * 
             * @return MyComplexArray 
             */
            MyComplexArray
            getOutput();

            /**
             * @brief Get the Inverse of the performed FFT
             * 
             * @return MyComplexArray 
             */
            MyComplexArray
            getInverse();

        private:
            /**
             * @brief Input array
             * 
             */
            const MyComplexArray input_data;
            /**
             * @brief array storing the calculated output
             * 
             */
            MyComplexArray output_data;

            bool eval_inverse;

            /**
             * @brief Compute the bit reverse operation on x
             * 
             * @param x the number to be reversed
             * @param log2n the log of the input size
             * @return const int the reversed number
             */
            const int 
            bitReverse(unsigned int x, const int log2n) const;

    };
}
#endif