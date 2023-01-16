#include <complex>
#include <cmath>
#include <valarray>

typedef std::complex<double> Complex;
typedef std::valarray<std::complex<double>> ComplexArray;

class MyComplexArray : public ComplexArray {
    public:
        // Default constructor
        MyComplexArray() {}

        // Constructor with size
        MyComplexArray(size_t size) : ComplexArray(size) {}

        // Constructor with size and initial value
        MyComplexArray(size_t size, Complex val) : ComplexArray(size) {
            std::fill(&this->operator[](0), &this->operator[](size), val);
        }

        // Copy constructor
        MyComplexArray(const MyComplexArray& other) : ComplexArray(other) {}

        operator ComplexArray() const { return ComplexArray(*this); }

        void operator*=(const MyComplexArray& other) {
            for(size_t i = 0; i< this->size(); i++){
                (*this)[i] = Complex(((*this)[i].real() * other[i].real()), ((*this)[i].imag() * other[i].imag()));
            }
        }


        MyComplexArray& operator*(const MyComplexArray& other) {
            MyComplexArray ret(other.size());
            for(size_t i = 0; i< this->size(); i++){
                ret[i] = Complex(((*this)[i].real() * other[i].real()), ((*this)[i].imag() * other[i].imag()));
            }
            return ret;
        }

        MyComplexArray& apply() {
            for (size_t i = 0; i < this->size(); i++) {
                (*this)[i] = Complex((*this)[i].real(), -(*this)[i].imag());
            }
            return *this;
        }

};
