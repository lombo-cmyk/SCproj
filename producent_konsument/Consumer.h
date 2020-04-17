//
// Created by lkania on 17.04.2020.
//

#ifndef PRODUCENT_KONSUMENT_CONSUMER_H
#define PRODUCENT_KONSUMENT_CONSUMER_H

#include "Queue.h"

class Consumer{
public:
    std::shared_ptr<Queue> m_queue;
    Consumer() = default;
    Consumer(std::shared_ptr<Queue> passed_queue);

    void TakeAndSort();
    double CalculateAverage(std::array<int, Queue::arrSize> passedArray);
};



#endif //PRODUCENT_KONSUMENT_CONSUMER_H
