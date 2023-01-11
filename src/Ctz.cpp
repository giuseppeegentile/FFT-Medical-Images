#include "Ctz.hpp"
#include <iostream>
namespace Solver{
    using namespace Solver;

    const int Ctz::nearestPowerOf2(int n) const {
        n--;
        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;
        n++;
        return n;
    }


    const  ComplexArray Ctz::solve(bool inverse) const {
        const int n = input_data.size();
        const int m =  nearestPowerOf2(2 * n +1);

        // Trignometric table
        ComplexArray omegas(n);
        const double coef = M_PI / n * (inverse ? 1 : -1);
        #pragma omp parallel for schedule(dynamic, 2)
        for (int i = 0; i < n; i++) {
            const int j = (int)((long)i * i % (n * 2));  // This is more accurate than j = i * i
            omegas[i] = std::exp(Complex(0, j * coef));
        }

        // Temporary vectors and preprocessing
        ComplexArray a(m);
        ComplexArray b(m);
        
        #pragma omp parallel for schedule(dynamic, 2)
        for (int i = 0; i < n; i++)
            a[i] = input_data[i] * omegas[i];

        
        b[0] = omegas[0];
        //reverse array and conjugate
        for (int i = 1; i < n; i++)
            b[i] = b[m - i] = std::conj(omegas[i]);
        // Convolution (using fourier transform and then dot product to have better performance)
        ComplexArray convolved(m);
        double s = omp_get_wtime();
        convolved = convolve(a, b);
        double e = omp_get_wtime();

        std::cout <<"Conv: " << e - s << std::endl;

        ComplexArray res(n);
        #pragma omp parallel for schedule(dynamic, 2)
        for (int i = 0; i < n; i++)
            res[i] = convolved[i] * omegas[i];
        return res;
    }


    /* 
    * Convolution theorem
    * Computes the circular convolution of the given complex vectors. 
    */
    const ComplexArray Ctz::convolve(ComplexArray& xvector,  ComplexArray& yvector) const {
        //Transform(xvector, false);
        FFT fft_x(xvector);
        fft_x.solveIterative(false);
        xvector = fft_x.getOutput();

        FFT fft_y(yvector);
        fft_y.solveIterative(false);
        yvector = fft_y.getOutput();

        xvector *= yvector;
        FFT inv(xvector);
        ComplexArray outvector = inv.getInverse();
        return outvector;
    }

}
