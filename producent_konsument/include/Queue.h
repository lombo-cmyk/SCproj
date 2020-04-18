//
// Created by lkania on 17.04.2020.
//

#ifndef PRODUCENT_KONSUMENT_QUEUE_H
#define PRODUCENT_KONSUMENT_QUEUE_H

#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>


class Queue{
public:
    int m_length;
    static const int arrSize = 100000;
    std::vector<std::array<int, arrSize>> m_queueVector;
    std::mutex m_queMutex;
    std::mutex m_printMutex;
    std::condition_variable m_conditionVariable;
    std::atomic<int> m_numberOfConsumedArrays = 0;
    explicit Queue(int queue_length):m_length(queue_length) {

    }

};

#endif //PRODUCENT_KONSUMENT_QUEUE_H
