#pragma once

#include <iostream>
#include <cstring>

namespace Tools{
    char* getChar(const std::string fn ) {
        const int length = fn.length();
        char* c = new char[length + 1]; //+1 fot the \n
        strcpy(c, fn.c_str());
        return c;
    }
}
