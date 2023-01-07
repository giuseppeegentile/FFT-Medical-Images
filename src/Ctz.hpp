#include "FFT_Traits.hpp"
#include "FFT.hpp"
#include <algorithm>

namespace Solver{

    class Ctz{

        public:
            Ctz(const ComplexArray &input_data_):input_data(input_data_) {}


            const ComplexArray solve(bool inverse) const;

            
        private:
            //array storing the input
            ComplexArray input_data;
            const ComplexArray convolve( ComplexArray& xvector,  ComplexArray& yvector) const;
            const int nearestPowerOf2(int n) const;
    };
}
