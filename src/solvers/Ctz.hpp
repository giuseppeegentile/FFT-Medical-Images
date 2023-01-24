#include "../traits/FFT_Traits.hpp"
#include "../solvers/FFT.hpp"
#include <algorithm>

namespace Solver{
    /**
     * Not used since the dataset is 256x256 image size, therefore the standard FFT is more efficient than the bluestein algorithm implemented here.
    */
    class Ctz{
        public:
            Ctz(const MyComplexArray &input_data_):input_data(input_data_) {}
            const MyComplexArray solve(bool inverse) const;
        private:
            /**
             * array storing the input
             */
            MyComplexArray input_data;
            const MyComplexArray convolve( MyComplexArray& xvector,  MyComplexArray& yvector) const;
            const int nearestPowerOf2(int n) const;
    };
}
