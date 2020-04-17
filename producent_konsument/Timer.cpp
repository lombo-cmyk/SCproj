//
// Created by lkania on 17.04.2020.
//

#include "Timer.h"
#include <iostream>
Timer::Timer(): start(std::chrono::system_clock::now()) {

}

Timer::~Timer() {
    stop = std::chrono::system_clock::now();
    std::chrono::duration<double> duration=stop - start;
    std::cout << "Program finished in: " << duration.count() << std::endl;

}