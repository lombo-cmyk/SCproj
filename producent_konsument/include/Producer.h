//
// Created by lkania on 17.04.2020.
//
#ifndef PRODUCENT_KONSUMENT_PRODUCER_H
#define PRODUCENT_KONSUMENT_PRODUCER_H


#include <random>
#include "types.h"
#include "Queue.h"


class Producer {
public:
    Producer(std::shared_ptr <Queue> passedQueue, int passedNumberOfArrays);
    void AddElementsToQueue() const;

private:
    const std::size_t m_maxToDraw = 50000;
    const std::size_t m_minToDraw = -50000;
    std::shared_ptr <Queue> m_queue;
    int m_numberOfArrays;
    void FillArray(intArray& qElement, std::uniform_int_distribution<> dis, std::mt19937& gen) const;
};

#endif //PRODUCENT_KONSUMENT_PRODUCER_H
