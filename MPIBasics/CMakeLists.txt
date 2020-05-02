cmake_minimum_required(VERSION 3.15)
project(MPIBasics)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "src/*.cpp")
add_executable(MPIBasics ${SRC})


