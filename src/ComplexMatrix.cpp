#include <complex>
#include <iostream>
#include <vector>
#include <valarray>

#include "ComplexMatrix.hpp"



/*
ComplexMatrix::ComplexMatrix(const ComplexMatrix& mat){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            storage[i * width + j] = mat.storage[i * width + j];
        }
    }
}*/

void ComplexMatrix::add(const size_t row, ComplexArray vals){
    for(size_t i = 0;  i < width; i++)
        storage[row * width + i] = vals[i];
}

void ComplexMatrix::add(const size_t row,const size_t col, Complex val){
    storage[row * width + col] = val;
}

ComplexArray ComplexMatrix::getRow(const size_t row){
    ComplexArray ret;
    ret.resize(width);
    for(size_t i = 0;  i < width; i++)
        ret[i] = storage[row * width + i];
    return ret;
}



/*
ComplexMatrix::~ComplexMatrix() {
    free(&storage);
}*/


ComplexMatrix ComplexMatrix::transpose() {
    ComplexMatrix ret_matrix(width, height);
    //this may work only for square matrix
    #pragma omp parallel for schedule(dynamic, 2) //collapse(2)
    for (size_t i = 0; i < getHeight(); ++i){
        ret_matrix.add(i, i, storage[i * width + i]);
        for (size_t j = i + 1; j < getWidth(); ++j) {
            //input_data.add(j, i, in(i, j));
            ret_matrix.add(j, i, storage[i * width + j]);
            ret_matrix.add(i, j, storage[j * width + i]);
        }
    }
/*      This work also for rectangular
    #pragma omp  parallel for schedule(dynamic, 2) collapse(2) //try task
    for (size_t i = 0; i < in.getHeight(); ++i){
        for (size_t j = 0; j < in.getWidth(); ++j) {
            input_data.add(j, i, in(i, j));
        }
    }*/
    return ret_matrix;
}

void ComplexMatrix::dotProduct(ComplexMatrix kernel, ComplexMatrix& res ) {
    for(size_t i = 0; i < height; i++){
        for(size_t j = 0; j < width; j++){
            res.add(i, j, (storage[i * width + j]) * kernel(i, j));
        }
    }
}

const size_t ComplexMatrix::getWidth() const { return width; }

const size_t ComplexMatrix::getHeight() const {return height; }


void ComplexMatrix::addColMajor(const size_t col, ComplexArray vals){
    for(size_t r = 0; r < height; r++){
        add(r, col, vals[r]);
    }
}


const void ComplexMatrix::print() const{
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++)
            std::cout << storage[i * width + j] << ", ";
        std::cout << std::endl;
    }
}

const ComplexMatrix ComplexMatrix::magnitude(ComplexMatrix& other) const{
    ComplexMatrix res(width, height);
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++)
            res.add(i, j, std::hypot(this->get(i,j).real(), other.get(i, j).real()));
    }
}

void ComplexMatrix::normalize(const Complex& val) {
    for(int i = 0; i < getHeight(); i++){
        for(int j = 0; i < getWidth(); j++){
            storage[i * width + j] /= val;
        }
    }
}