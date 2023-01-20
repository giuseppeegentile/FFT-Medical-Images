cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
#set(VTK_DIR /usr/local/lib/VTK/build/lib/cmake/vtk-9.2)

#set(VTK_DIR ${mkVtkPrefix})

#project name
project(FFT CXX)

find_package(OpenMP REQUIRED)
find_package(VTK REQUIRED HINTS $ENV{mkVtkPrefix})
add_compile_options(-std=c++17 -Wall -fopenmp )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/medical_images/stanford)

#add executable
add_executable(FFT src/solvers/FFT.cpp  src/types/ComplexMatrix.cpp src/solvers/MDFFT.cpp  src/solvers/Ctz.cpp src/imaging/Image.cpp  src/main.cpp )

target_link_libraries(FFT PUBLIC OpenMP::OpenMP_CXX ${VTK_LIBRARIES})

