//
// Created by lkania on 17.04.2020.
//

#ifndef PRODUCENT_KONSUMENT_CONSUMER_H
#define PRODUCENT_KONSUMENT_CONSUMER_H

#include "Queue.h"

typedef std::array<int, Queue::arrSize> intArray;

class Consumer{
public:
    Consumer() = default;
    Consumer(std::shared_ptr<Queue> passed_queue);
    void TakeAndSort();

private:
    std::shared_ptr<Queue> m_queue;
    double CalculateAverage(intArray passedArray);
};



#endif //PRODUCENT_KONSUMENT_CONSUMER_H
