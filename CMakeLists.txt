cmake_minimum_required(VERSION 3.22)
project(PP_bigram_omp)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

find_package(Python3 COMPONENTS Interpreter Development NumPy)

include_directories(${Python3_INCLUDE_DIRS} include)

add_executable(PP_bigram_omp main.cpp)

target_link_libraries(PP_bigram_omp ${Python3_LIBRARIES})

