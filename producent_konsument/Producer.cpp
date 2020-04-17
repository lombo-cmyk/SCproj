//
// Created by lkania on 17.04.2020.
//

#include "Producer.h"
#include <utility>
#include <random>
#include <algorithm>
#include <mutex>
#include <condition_variable>

Producer::Producer(std::shared_ptr<Queue> passedQueue, int passedNumberOfArrays) :
    m_queue{std::move(passedQueue)}, m_numberOfArrays(passedNumberOfArrays){

    }

void Producer::AddElementsToQueue() {
    std::random_device rd;
    std::mt19937 gen(rd());
    int absMaxToDraw = 50000;
    std::uniform_int_distribution<> dis(-absMaxToDraw, absMaxToDraw);
    int j=0;
    intArray queueElement ={};
//    std::array<int, Queue::arrSize> queueElement={};
    while(j < m_numberOfArrays) {
        if(m_queue->m_queueVector.size() < m_queue->m_length){
            j++;
            for (int i = 0; i < Queue::arrSize; i++) {
                queueElement[i] = dis(gen);
            }
            {
                std::lock_guard<std::mutex> lock(m_queue->m_queMutex);
                m_queue->m_queueVector.push_back(queueElement);
            }
            m_queue->m_conditionVariable.notify_one();
        }
        else{
            m_queue->m_conditionVariable.notify_one();
            std::this_thread::yield();
        }

    }
    std::lock_guard<std::mutex> lock(m_queue->m_printMutex);
    std::cout << "PRODUCER FINISHED" << std::endl;
}
