# Some Notes #
- Setting the value returned by a function as const is completely useless (and indeed meaningless since the returned value is a temporary object). It is not an error because the compiler discards the const automatically, but it is just  a waste:

````
const size_t ComplexMatrix::getWidth() const { return width; }
````
should be just
````
size_t ComplexMatrix::getWidth() const { return width; }
````
It happenss in several places.
- Code is well commented. Good. Very good programming. Rather complex code.
- Code compiles and runs without issues, you provide very interesting examples and a complete doxygen documentation
- I don't understand the need of `Tools::getChar(std::string)` to convert a C++ string to a `char *`.
Can't you use the method `c_str()` directly?


# Minor stuff #
- A few c header files that could have been changed to the coresponding c++ sdtint.h -> <cstdint>, 
and string.h -><string> 
- In some classes data member are left uninitilised at construction. Maybe it is fine, but I suggest to give always 
an initial value to POD data, don;t assume they are initialized by zero. Better safe than sorry.
- If you use  `std::string volume` it is safer to test for an empty string using `if (volume.empty())`
than `volume ==""`. 
- Again for safety: when you override a virtual method, use the `override` specifier. Not compulsory but highly 
suggested.
- You sometimes forgot header guards in `*.hpp` files.
- In `Image.hpp`, you should use `const std::string &` instead of `const char *`
- Do not use `NULL` (which is a macro for 0) but `nullptr`
- In `Image.cpp` you use many C-style casts for integers, this is not an error but `static_cast` should be preferred. Moreover, (if possible) try to use always the same integral type to indicate indexes instead of keep changing it among `int`, `long` and `uint64_t`.
