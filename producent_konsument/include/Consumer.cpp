//
// Created by lkania on 17.04.2020.
//
#include "Consumer.h"
#include <mutex>
#include <condition_variable>
#include <numeric>

Consumer::Consumer(std::shared_ptr<Queue> passed_queue) :
m_queue(std::move(passed_queue)){

}

double Consumer::CalculateAverage(intArray& passedArray) const {
    int sum = std::accumulate(passedArray.begin(), passedArray.end(), 0);
    return static_cast<double>(sum)/passedArray.size();
}

void Consumer::TakeAndSort() const {
    int numberOfConsumedArraysByMe=0;
    intArray queueElement={};
    double average;
    while(true){
        std::unique_lock<std::mutex> lck(m_queue->m_queMutex);
        if (!m_queue->m_queueVector.empty()) {
            queueElement = m_queue->m_queueVector.front();
            m_queue->m_queueVector.erase(m_queue->m_queueVector.begin());
            lck.unlock();
            m_queue->m_numberOfConsumedArrays++;
            numberOfConsumedArraysByMe++;
            std::sort(queueElement.begin(), queueElement.end());
            average = CalculateAverage(queueElement);
            std::lock_guard<std::mutex> lock(m_queue->m_printMutex);
            std::cout << "Calculated checksum: " << average << std::endl;
        }
        else {
            std::this_thread::yield();
            if(m_queue->m_conditionVariable.wait_for(lck,
                                                     std::chrono::seconds(1))==std::cv_status::timeout) break;
        }
    }
    std::lock_guard<std::mutex> lock(m_queue->m_printMutex);
    std::cout << "I consumed: " << numberOfConsumedArraysByMe << " arrays" << std::endl;
}


