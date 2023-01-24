#pragma once

#include <iostream>
#include <cstring>

namespace Tools{
    /**
     * @brief Get the Char conversion of the std::string type. The library to read/write images takes in input char* filenames
     * 
     * @param fn the string filename
     * @return char* 
     */
    char* getChar(const std::string fn ) {
        const int length = fn.length();
        char* c = new char[length + 1]; //+1 fot the \n
        strcpy(c, fn.c_str());
        return c;
    }
}
