cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

#project name
project(FFT CXX)

find_package(OpenMP REQUIRED)
add_compile_options(-std=c++11 -Wall -fopenmp -g)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/csv)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/images)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/medical_images/stanford)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/processed_medical_images)

#add executable
add_executable(FFT src/FFT.cpp  src/ComplexMatrix.cpp src/MDFFT.cpp  src/CSV_Reader.cpp src/Ctz.cpp src/Image.cpp  src/main2.cpp )

target_link_libraries(FFT PUBLIC OpenMP::OpenMP_CXX)

