#include <iostream>
#include <array>
#include <utility>
#include <thread>
#include <condition_variable>
#include "Queue.h"
#include "Producer.h"
#include "Consumer.h"
#include "Timer.h"

int main() {
    /*8 concurrent threads are supported.*/
    /*4 Physical cores*/
    Timer T;
    int queueLength = 200;
    int numberOfArrays = 4000;

    auto myQueue=std::make_shared<Queue>(queueLength);
    Producer myProducer(myQueue, numberOfArrays);

    const std::size_t NumberOfThreads=7;
    std::array<Consumer, NumberOfThreads> myConsumers={};
    for (auto& consumer : myConsumers){
        consumer=myQueue;
    }
    std::array<std::thread, NumberOfThreads> ConsumerThread;
    std::thread ProducerThread(&Producer::AddElementsToQueue, myProducer);
    for(int j=0; j<NumberOfThreads; j++){
        ConsumerThread[j]=std::thread(&Consumer::TakeAndSort, myConsumers[j]);
    }
    ProducerThread.join();
    for (auto &Thread : ConsumerThread)
        Thread.join();

    std::cout << "No all consumed arrays <atomic>: " << myQueue->m_numberOfConsumedArrays << std::endl;
}
