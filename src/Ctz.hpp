#include "FFT_Traits.hpp"
#include "FFT.hpp"
#include <algorithm>

namespace Solver{

    class Ctz{

        public:
            Ctz(const MyComplexArray &input_data_):input_data(input_data_) {}


            const MyComplexArray solve(bool inverse) const;

            
        private:
            //array storing the input
            MyComplexArray input_data;
            const MyComplexArray convolve( MyComplexArray& xvector,  MyComplexArray& yvector) const;
            const int nearestPowerOf2(int n) const;
    };
}
