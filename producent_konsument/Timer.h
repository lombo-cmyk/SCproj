//
// Created by lkania on 17.04.2020.
//

#ifndef PRODUCENT_KONSUMENT_TIMER_H
#define PRODUCENT_KONSUMENT_TIMER_H

#include <utility>
#include <chrono>
class Timer{
public:
    Timer();
    ~Timer();

private:
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point stop;

};


#endif //PRODUCENT_KONSUMENT_TIMER_H
