#ifndef HH_READCVSHEADER_HH
#define HH_READCVSHEADER_HH

#include <iostream>
#include <vector>
#include <cmath>

#include <fstream>
#include <istream>
#include <string>

#include <complex>
#include <valarray>

#pragma once
#include "FFT_Traits.hpp"

namespace Utility {
    typedef std::vector<std::vector<std::string>> Table;
    typedef std::ifstream File;
    typedef std::vector<std::string> Row;
    typedef std::valarray<std::complex<double>> ComplexArray;
    typedef std::ofstream OutputFile;

    class CSV_Reader{
        public:
            CSV_Reader(File &_file):file(_file) { }
            
            const  void
            readCSV();

            const int 
            getArraySize();

            const MyComplexArray
            getArray() const;


            int
            getMatrixColumn();

        private:
            enum class CSVState {
                UnquotedField,//read char
                QuotedField,//go new line
            };
            File &file;      

            Table table;

            const Row readCSVRow(const std::string &row) const;
    };
}
#endif