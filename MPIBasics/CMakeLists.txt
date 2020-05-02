cmake_minimum_required(VERSION 3.15)
project(MPIBasics)

set(CMAKE_CXX_STANDARD 20)

add_executable(MPI src/main.cpp src/MyMpi.cpp)

