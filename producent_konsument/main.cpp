#include <iostream>
#include <thread>
#include <array>
#include "include/Queue.h"
#include "include/Producer.h"
#include "include/Consumer.h"
#include "include/Timer.h"


int main() {
    /*8 concurrent threads are supported.*/
    /*4 Physical cores*/
    Timer T;
    const std::size_t queueLength = 200;
    const std::size_t numberOfArrays = 4000;
    const std::size_t NumberOfConsumerThreads=7;

    auto myQueue=std::make_shared<Queue>(queueLength);
    Producer myProducer(myQueue, numberOfArrays);

    std::array<Consumer, NumberOfConsumerThreads> myConsumers={};
    for (auto& consumer : myConsumers){
        consumer=myQueue;
    }
    std::array<std::thread, NumberOfConsumerThreads> ConsumerThread;
    std::thread ProducerThread(&Producer::AddElementsToQueue, myProducer);
    for(int j=0; j < NumberOfConsumerThreads; j++){
        ConsumerThread[j]=std::thread(&Consumer::TakeAndSort, myConsumers[j]);
    }
    ProducerThread.join();
    for (auto &Thread : ConsumerThread)
        Thread.join();

    std::cout << "No all consumed arrays <atomic>: " << myQueue->m_numberOfConsumedArrays << std::endl;
}
