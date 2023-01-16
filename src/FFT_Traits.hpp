/*
 * FFT_traits.hpp
 *
 *  Created on: Dec 14, 2022
 *      Author: forma
 */

#ifndef FFT2_FFT_AMSC_SRC_FFT_TRAITS_HPP_
#define FFT2_FFT_AMSC_SRC_FFT_TRAITS_HPP_
#include <omp.h>
#include <complex>
#include <cmath>
#include <valarray>
#include "MyComplexArray.hpp"
typedef std::complex<double> Complex;
typedef std::valarray<Complex> ComplexArray;
constexpr auto epsilon = std::numeric_limits<double>::epsilon();

#endif /* FFT2_FFT_AMSC_SRC_FFT_TRAITS_HPP_ */