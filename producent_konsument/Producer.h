//
// Created by lkania on 17.04.2020.
//
#ifndef PRODUCENT_KONSUMENT_PRODUCER_H
#define PRODUCENT_KONSUMENT_PRODUCER_H

#include <iostream>
#include <thread>
#include "Queue.h"

class Producer {
public:
    std::shared_ptr <Queue> m_queue;
    int m_numberOfArrays;

    Producer(std::shared_ptr <Queue> passedQueue, int passedNumberOfArrays);

    void AddElementsToQueue();
};

#endif //PRODUCENT_KONSUMENT_PRODUCER_H
