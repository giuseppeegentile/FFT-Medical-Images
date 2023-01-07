#include "CSV_Reader.hpp"


/************************
 * CREDITS: 
 * https://stackoverflow.com/a/30338543/7415123
 * Author: https://stackoverflow.com/users/25450/sastanin
 * 
 * 
 * Modications: made it specific for complex array/matrix reads

*/


namespace Utility{
    const Row 
    CSV_Reader::readCSVRow(const std::string &row) const {
        CSVState state = CSVState::UnquotedField;
        Row fields {""};
        size_t i = 0; // index of the current field
        for (char c : row) {
            switch (state) {
                case CSVState::UnquotedField:
                    switch (c) {
                        case ',': // end of field
                                fields.push_back(""); i++;
                                break;
                        case '"': state = CSVState::QuotedField;
                                break;
                        default:  fields[i].push_back(c);
                                break; }
                    break;
                case CSVState::QuotedField:
                    switch (c) {
                        default:  fields[i].push_back(c);
                                break; }
                    break;
            }
        }
        return fields;
    }

    //Excel format
    const void 
    CSV_Reader::readCSV() {
        std::string row;
        while (!file.eof()) {
            std::getline(file, row);
            if (file.bad() || file.fail()) break;
            auto fields = readCSVRow(row);
            table.push_back(fields);
        }
    }


    const int 
    CSV_Reader::getArraySize() {
        CSV_Reader::readCSV();
        return std::stoi(table.at(0).at(0));
    }

    const ComplexArray 
    CSV_Reader::getArray() const {
        ComplexArray input;

        const int size = std::stoi(table.at(0).at(0));
        input.resize(size);
        for(int i = 1; i < size + 1; i++){
            input[i - 1] = std::complex<double>(std::stod(table.at(i).at(0)), std::stod(table.at(i).at(1)));
        }
        return input;
    }

    ComplexMatrix
    CSV_Reader::getMatrix() {
        CSV_Reader::readCSV();
        const size_t row = std::stoi(table.at(0).at(0));
        const size_t col = std::stoi(table.at(0).at(1));
        ComplexMatrix input(row, col);
        for(size_t i = 0; i < row; i++){
            for(size_t j = 0, h = 0; j < col; j++, h++){
                input.add(i,j, std::complex<double>(std::stod(table.at(i + 1).at(h)), std::stod(table.at(i + 1).at(h + 1))));
                h++;
            }
        }
        return input;
    }
}