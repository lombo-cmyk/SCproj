//
// Created by lkania on 17.04.2020.
//
#ifndef PRODUCENT_KONSUMENT_PRODUCER_H
#define PRODUCENT_KONSUMENT_PRODUCER_H

#include <iostream>
#include <thread>
#include <utility>
#include <random>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include "Queue.h"

typedef std::array<int, Queue::arrSize> intArray;

class Producer {
public:
    Producer(std::shared_ptr <Queue> passedQueue, int passedNumberOfArrays);
    void AddElementsToQueue();

private:
    std::shared_ptr <Queue> m_queue;
    int m_numberOfArrays;
    intArray FillArray(std::uniform_int_distribution<> dis, std::mt19937& gen);
};

#endif //PRODUCENT_KONSUMENT_PRODUCER_H
