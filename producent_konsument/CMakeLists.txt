cmake_minimum_required(VERSION 3.15)
project(producent_konsument)



set(CMAKE_CXX_STANDARD 17)

add_executable(producent_konsument main.cpp Queue.h Producer.h Producer.cpp Consumer.h Consumer.cpp Timer.h)
target_link_options(producent_konsument PRIVATE -pthread)