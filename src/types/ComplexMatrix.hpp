#include <complex>
#include <iostream>
#include <vector>
#include <valarray>
#pragma once
#include "../traits/FFT_Traits.hpp"


/**
 * @brief Defines the object for a matrix with complex values.
 * 
 */
class ComplexMatrix {
    public:
        /**
         * @brief Construct a new ComplexMatrix object
         * 
         * @param width_ matrix width
         * @param height_ matrix height
         */
        ComplexMatrix(size_t width_, size_t height_): width(width_), height(height_) {
            storage.resize(width*height);
        }

        //ComplexMatrix(const ComplexMatrix& mat);

        /**
         * @brief getter of the element row-col using (row, col)
         * 
         * @param row 
         * @param column 
         * @return const Complex& 
         */
        const Complex &operator()(const size_t row, const size_t column) const {
            return storage[row * width + column]; // row major
        }

        /**
         * @brief Equivalent of the &operator()
         * 
         * @param row 
         * @param column 
         * @return const Complex 
         */
        inline  const Complex get(const size_t row, const size_t column) const { return storage[row * width + column];}

        /**
         * @brief Add in row-major a MyComplexArray
         * 
         * @param row the row where to add 
         * @param vals the MyComplexArray to add
         */
        void add(const size_t row, MyComplexArray vals);

        /**
         * @brief Add in col-major a MyComplexArray
         * 
         * @param col the col where to add  
         * @param vals the MyComplexArray to add
         */
        void addColMajor(const size_t col, MyComplexArray vals);

        /**
         * @brief Add a single element
         * 
         * @param row the row where to add
         * @param col the col where to add
         * @param val the val to add at row-col position
         */
        void add(const size_t row,const size_t col, Complex val);

        /**
         * @brief Get a Row of the matrix
         * 
         * @param row 
         * @return MyComplexArray 
         */
        MyComplexArray getRow(const size_t row);

        /**
         * @brief Get the Width of the matrix
         * 
         * @return const size_t 
         */
        const size_t getWidth() const;
        /**
         * @brief Get the Height of the matrix
         * 
         * @return const size_t 
         */
        const size_t getHeight() const;

        /**
         * @brief Perform the matrix transpose
         * 
         * @return ComplexMatrix the transposed matrix
         */
        ComplexMatrix transpose();

        /**
         * @brief Get the Storage (linearized matrix as MyComplexArray)
         * 
         * @return const MyComplexArray 
         */
        inline const MyComplexArray getStorage() const {return storage;};
        
        //const void print() const;

        /**
         * @brief Perform element-wise product
         * 
         * @param kernel the matrix to multiply with this
         * @param res the result of the product
         */
        void dotProduct(ComplexMatrix kernel, ComplexMatrix& res );

    private:
        MyComplexArray storage;
        size_t width;
        size_t height;
};
