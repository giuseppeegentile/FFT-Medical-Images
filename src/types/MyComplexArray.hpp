#include <complex>
#include <cmath>
#include <valarray>

typedef std::complex<double> Complex;
typedef std::valarray<std::complex<double>> ComplexArray;


/**
 * @brief A more confortable version of std::valarray<std::complex<double>>. Has the constructor with initial values (more similar to std::vector)
 * 
 */
class MyComplexArray : public ComplexArray {
    public:
        /**
         * @brief Default costructor
         * 
         */
        MyComplexArray() {}

        /**
         * @brief Constructor with size
         * 
         * @param size 
         */
        MyComplexArray(size_t size) : ComplexArray(size) {}

        /**
         * @brief Constructor with size and initial value
         * 
         * @param size size of the object
         * @param val all element of the object will be initialize as val
         */
        MyComplexArray(size_t size, Complex val) : ComplexArray(size) {
            std::fill(&this->operator[](0), &this->operator[](size), val);
        }

        /**
         * @brief Copy costructor
         * 
         * @param other the copied object
         */
        MyComplexArray(const MyComplexArray& other) : ComplexArray(other) {}

        //operator ComplexArray() const { return ComplexArray(*this); }

        /**
         * @brief Vector conjugate
         * 
         * @return MyComplexArray& 
         */
        MyComplexArray& apply() {
            for (size_t i = 0; i < this->size(); i++) {
                (*this)[i] = Complex((*this)[i].real(), -(*this)[i].imag());
            }
            return *this;
        }

};
