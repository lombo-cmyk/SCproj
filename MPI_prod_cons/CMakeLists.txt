cmake_minimum_required(VERSION 3.15)
project(MPI_prod_cons)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "src/*.cpp")
add_executable(MPI_prod_cons ${SRC})