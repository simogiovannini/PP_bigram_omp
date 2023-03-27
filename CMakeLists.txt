cmake_minimum_required(VERSION 3.22)
project(PP_bigram_omp)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(PP_bigram_omp main.cpp)
