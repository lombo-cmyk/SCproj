//
// Created by lkania on 17.04.2020.
//

#ifndef PRODUCENT_KONSUMENT_TIMER_H
#define PRODUCENT_KONSUMENT_TIMER_H

#include <chrono>
#include <iostream>

class Timer{
public:
    Timer(): start(std::chrono::system_clock::now()) {
    }
    ~Timer(){
        stop = std::chrono::system_clock::now();
        std::chrono::duration<double> duration=stop - start;
        std::cout << "Program finished in: " << duration.count() << std::endl;
    }

private:
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point stop;

};


#endif //PRODUCENT_KONSUMENT_TIMER_H
