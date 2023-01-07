#include <complex>
#include <iostream>
#include <vector>
#include <valarray>
#pragma once
typedef std::complex<double> Complex;
typedef std::valarray<Complex> ComplexArray;

class ComplexMatrix {
    public:
        ComplexMatrix(size_t width_, size_t height_): width(width_), height(height_) {
            storage.resize(width*height);
        }

        //ComplexMatrix(const ComplexMatrix& mat);

        Complex &operator()(const size_t row, const size_t column) {
            return storage[row * width + column]; // row major
        }

        inline Complex get(const size_t row, const size_t column) { return storage[row * width + column];}

        void add(const size_t row, ComplexArray vals);

        void addColMajor(const size_t row, ComplexArray vals);

        void add(const size_t row,const size_t col, Complex val);

        ComplexArray getRow(const size_t row);

        const size_t getWidth() const ;

        const size_t getHeight() const ;

        ComplexMatrix transpose();

        inline ComplexArray getStorage() {return storage;};
        

        void dotProduct(ComplexMatrix kernel, ComplexMatrix& res );


    private:
        ComplexArray storage;
        size_t width;
        size_t height;
};
