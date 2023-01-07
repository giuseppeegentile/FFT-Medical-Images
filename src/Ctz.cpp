#include "Ctz.hpp"

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
            const int m =  nearestPowerOf2(2* n +1);

            // Trignometric table
            ComplexArray omegas(n);
            const double coef = M_PI / n * (inverse ? 1 : -1);
            for (int i = 0; i < n; i++) {
                const int j = (int)((long)i * i % (n * 2));  // This is more accurate than j = i * i
                omegas[i] = std::exp(Complex(0, j * coef));
            }

            // Temporary vectors and preprocessing
            ComplexArray a(m);
            for (int i = 0; i < n; i++)
                a[i] = input_data[i] * omegas[i];

            ComplexArray b(m);
            b[0] = omegas[0];
            for (int i = 1; i < n; i++)
                b[i] = b[m - i] = std::conj(omegas[i]);

            // Convolution
            ComplexArray convolved(m);
            convolved = convolve(a, b);

            ComplexArray res(n);
            for (int i = 0; i < n; i++)
                res[i] = convolved[i] * omegas[i];
            return res;
        }


        /* 
         * Computes the circular convolution of the given complex vectors. Each vector's length must be the same.
         */
        const ComplexArray Ctz::convolve(ComplexArray& xvector,  ComplexArray& yvector) const {
            //Transform(xvector, false);
            FFT fft_x(xvector);
            fft_x.solveIterative(true);
            xvector = fft_x.getOutput();

            FFT fft_y(yvector);
            fft_y.solveIterative(true);
            yvector = fft_y.getOutput();
            xvector *= yvector;
            FFT inv(xvector);
            ComplexArray outvector = inv.getInverse();
            return outvector;
        }

}

/*
    ComplexArray Ctz::bluestein(ComplexArray &a) {
        const int n = a.size();
        // Find the smallest power of 2 that is greater than or equal to n
        int m = 1;
        while (m < n) m <<= 1;

        // Allocate memory for the Bluestein's chirp and its inverse
        ComplexArray omega(m);
        ComplexArray omega_inv(m);

        // Compute the Bluestein's chirp and its inverse
        omega[0] = 1;
        omega_inv[0] = 1;
        for (int i = 1; i < m; i++) {
            omega[i] = std::exp(std::complex<double>(0, -2 * M_PI * i / m));
            omega_inv[i] = std::conj(omega[i]);
        }

        // Allocate memory for the padded input array and the transformed output array
        ComplexArray a_pad(m);
        ComplexArray b(m);

        // Pad the input array with zeros
        for (int i = 0; i < n; i++) a_pad[i] = a[i];
        for (int i = n; i < m; i++) a_pad[i] = 0;

        // Compute the convolution b = a_pad * omega
        ComplexArray b_pad(m);
        for (int i = 0; i < m; i++) b_pad[i] = a_pad[i] * omega[i];

        // Use the FFT to compute the convolution quickly
        FFT fft(b_pad);
        fft.solveIterative(true);
        b_pad = fft.getOutput();

        // Compute the transformed output array b = b_pad * omega_inv
        for (int i = 0; i < m; i++) b[i] = b_pad[i] * omega_inv[i];

        return b;
    }*/

    //ai 2 
    /*
    void bluestein(std::complex<double> *a, int n) {
        // Find the smallest power of 2 that is greater than or equal to n
        int m = 1;
        while (m < n) m <<= 1;

        // Allocate memory for the Bluestein's chirp and its inverse
        std::complex<double> *omega = new std::complex<double>[m];
        std::complex<double> *omega_inv = new std::complex<double>[m];

        // Compute the Bluestein's chirp and its inverse
        omega[0] = 1;
        omega_inv[0] = 1;
        for (int i = 1; i < m; i++) {
            omega[i] = std::exp(std::complex<double>(0, -2 * M_PI * i / m));
            omega_inv[i] = std::conj(omega[i]);
        }

        // Allocate memory for the padded input array and the transformed output array
        std::complex<double> *a_pad = new std::complex<double>[m];
        std::complex<double> *b = new std::complex<double>[m];

        // Pad the input array with zeros
        for (int i = 0; i < n; i++) a_pad[i] = a[i];
        for (int i = n; i < m; i++) a_pad[i] = 0;

        // Compute the convolution b = a_pad * omega
        std::complex<double> *b_pad = new std::complex<double>[m];
        for (int i = 0; i < m; i++) b_pad[i] = a_pad[i] * omega[i];

        // Use the FFT to compute the convolution quickly
        fft(b_pad, m);

        // Compute the transformed output array b = b_pad * omega_inv
        for (int i = 0; i < m; i++) b[i] = b_pad[i] * omega_inv[i];

        // Free memory
        delete[] omega;
        delete[] omega_inv;
        delete[] a_pad;
        delete[] b_pad;
    }*/


/*
    ComplexArray Ctz::solve() {
        const int n = input_data.size();
        const int m = 1 << (int)std::ceil(std::log2( 2 * n - 1));  
        ComplexArray a(m), b(m);
        for (int i = 0; i < n; i++) a[i] = input_data[i];
        for (int i = n; i < m; i++) a[i] = 0.;

        b[0] = Complex(1., 0.);
        for (int i = 1; i < m; i++) {
            double angle = M_PI / (i + i);
            b[i] = Complex(std::cos(angle), std::sin(angle));
        }
        for (int i = 0; i < m; i++) b[m - i - 1] *= b[i];
        //std::reverse(std::begin(b), std::end(b));


        ComplexArray y(m);
        for (int i = 0; i < m; i++) y[i] = b[i] * a[i];
        FFT fft(y);
        ComplexArray z = fft.getOutput();
        z /= m;
        ComplexArray result(n);
        for (int i = 0; i < n; i++) result[i] = z[i] * b[i];
        return result;
    }*/




/*#include <vector>
#include <algorithm>
#include <complex>
#include <cmath>
#include <cstdint>

using std::complex;

// Computes the element-wise product of two complex vectors.
std::vector<complex<double>> element_wise_product(const std::vector<complex<double>>& a,
                                                  const std::vector<complex<double>>& b)
{
    std::vector<complex<double>> result(a.size());
    std::transform(a.begin(), a.end(), b.begin(), result.begin(),
                   [](complex<double> x, complex<double> y) { return x * y; });
    return result;
}

// Computes the inverse discrete Fourier transform of the given complex vector.
std::vector<complex<double>> idft(const std::vector<complex<double>>& x, complex<double> w)
{
    uint64_t n = x.size();
    complex<double> w_inv = std::conj(w);
    std::vector<complex<double>> result(n);
    for (uint64_t k = 0; k < n; ++k)
    {
        result[k] = 0;
        for (uint64_t t = 0; t < n; ++t)
        {
            result[k] += x[t] * std::pow(w_inv, static_cast<double>(t * k));
        }
    }
    return result;
}

// Computes the discrete Fourier transform of the given complex vector.
std::vector<complex<double>> dft(const std::vector<complex<double>>& x, complex<double> w)
{
    uint64_t n = x.size();
    std::vector<complex<double>> result(n);
    for (uint64_t k = 0; k < n; ++k)
    {
        result[k] = 0;
        for (uint64_t t = 0; t < n; ++t)
        {
            result[k] += x[t] * std::pow(w, static_cast<double>(-t * k));
        }
    }
    return result;
}

// Computes the Bluestein algorithm for computing the circular convolution of the given
// complex vectors.
std::vector<complex<double>> bluestein(const std::vector<complex<double>>& a)
{
    uint64_t n = a.size();

    // Calculate basic values
    complex<double> xi = std::exp(complex<double>(0, M_TAU / n));

    // Calculate u(n) and v(n)
    std::vector<complex<double>> u(n);
    std::vector<complex<double>> v(n);
    std::vector<complex<double>> v_estrella(n);
    for (uint64_t i = 0; i < n; ++i)
    {
        u[i] = a[i] * std::pow(xi, -static_cast<double>(i * i) / 2);
        v[i] = std::pow
        */


