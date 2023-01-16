cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

#project name
project(FFT CXX)

find_package(OpenMP REQUIRED)
add_compile_options(-std=c++17 -g -pg -Wall -fopenmp )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/csv)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/images)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/medical_images/stanford)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/processed_medical_images/anisotropic)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/processed_medical_images/gaussed)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/processed_medical_images/kuwhara)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/processed_medical_images/NLMF)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/processed_medical_images/sobel)

#add executable
add_executable(FFT src/solvers/FFT.cpp  src/types/ComplexMatrix.cpp src/solvers/MDFFT.cpp src/Utilities/CSV_Reader.cpp src/solvers/Ctz.cpp src/imaging/Image.cpp  src/main.cpp )

target_link_libraries(FFT PUBLIC OpenMP::OpenMP_CXX)

