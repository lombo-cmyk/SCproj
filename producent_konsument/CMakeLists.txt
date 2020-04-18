cmake_minimum_required(VERSION 3.15)
project(producent_konsument)



set(CMAKE_CXX_STANDARD 17)

add_executable(producent_konsument main.cpp include/Queue.h include/Producer.h include/Producer.cpp include/Consumer.h include/Consumer.cpp include/Timer.h include/types.h)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_options(producent_konsument PRIVATE -pthread)
endif()
