#include <iostream>
#include <array>
#include <utility>
#include <vector>
#include <random>
#include <memory>
#include <algorithm>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>

#define ARR_SIZE 100000
#define QUEUE_LENGTH 200
#define NUMBER_OF_ARRAYS 4000


class Queue{
public:
    int m_length;
    std::vector<std::array<int, ARR_SIZE>> m_queueVector;
    std::mutex m_queMutex;
    std::mutex m_printMutex;
    std::condition_variable m_conditionVariable;
    std::atomic<int> m_numberOfConsumedArrays = 0;
    explicit Queue(int queue_length) {
        m_length=queue_length;
    }

};

class Producer{
public:
    std::shared_ptr<Queue> m_queue;
    int m_numberOfArrays;
    Producer(std::shared_ptr<Queue> passedQueue, int passedNumberOfArrays) :
    m_queue{std::move(passedQueue)}, m_numberOfArrays(passedNumberOfArrays){

    }

    void AddElementsToQueue(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(-50000, 50000);
        int j=0;
        std::array<int, ARR_SIZE> queueElement={};
        while(j < m_numberOfArrays) {
            if(m_queue->m_queueVector.size() < m_queue->m_length){
                j++;
                for (int i = 0; i < ARR_SIZE; i++) {
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
};

class Consumer{
public:
    std::shared_ptr<Queue> m_queue;
    Consumer() = default;
    Consumer(std::shared_ptr<Queue> passed_queue) :
    m_queue(std::move(passed_queue)){

    }

    void TakeAndSort(){
        int numberOfConsumedArraysByMe=0;
        std::array<int, ARR_SIZE> queueElement={};
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
                {
                    std::lock_guard<std::mutex> lock(m_queue->m_printMutex);
                    std::cout << "Calculated checksum: " << average << std::endl;
                }
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
    double CalculateAverage(std::array<int, ARR_SIZE> passedArray){
        double sum=0;
        for(auto& number : passedArray){
            sum+=number;
        }
        return sum/passedArray.size();
    }
};

int main() {
    /*8 concurrent threads are supported.*/
    /*4 Physical cores*/
    auto start=std::chrono::system_clock::now();

    auto myQueue=std::make_shared<Queue>(QUEUE_LENGTH);
    Producer myProducer(myQueue, NUMBER_OF_ARRAYS);

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
    auto stop=std::chrono::system_clock::now();
    std::chrono::duration<double> duration=stop-start;

    std::cout << "Program finished in: "<< duration.count() << std::endl;
    std::cout << "No all consumed arrays <atomic>: " << myQueue->m_numberOfConsumedArrays << std::endl;
    system("Pause");
}
