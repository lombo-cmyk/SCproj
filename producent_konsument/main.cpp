#include <iostream>
#include <array>
#include <vector>
#include <random>
#include <memory>
#include <algorithm>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>

#define ARRSIZE 100000
#define QUEUELENGHT 200
#define NUMBEROFARRAYS 4000

std::condition_variable conditionVariable;
std::mutex locker;
std::atomic<int> NumberOfConsumedArrays;

class Queue{
public:
    int q_lenght;
    std::vector<std::array<int, ARRSIZE>> queue_vector;
    Queue(int queue_lenght){
        q_lenght=queue_lenght;
    }
};

class Producer{
public:
    std::shared_ptr<Queue> queue;
    int number_of_arrays;
    Producer(std::shared_ptr<Queue> passed_queue, int passed_number_of_arrays){
        queue=passed_queue;
        number_of_arrays=passed_number_of_arrays;
    }

    void adding_elements_to_queue(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(-50000, 50000);
        int j=0;
        std::array<int, ARRSIZE> queue_element={};
        while(j<number_of_arrays) {
            if(queue.get()->queue_vector.size()<QUEUELENGHT){
                j++;
                for (int i = 0; i < ARRSIZE; i++) {
                    queue_element[i] = dis(gen);
                }
                {
                    std::lock_guard<std::mutex> lock(locker);
                    queue.get()->queue_vector.push_back(queue_element);
                }
                conditionVariable.notify_one();
            }
            else{
                conditionVariable.notify_one();
                std::this_thread::yield();
            }

        }
        std::cout << "PRODUCER FINISHED" << std::endl;
    }
};

class Consumer{
public:
    std::shared_ptr<Queue> queue;
    Consumer(std::shared_ptr<Queue> passed_queue){
     queue=passed_queue;
    }

    void take_and_sort(){
        int NumberOfConsumedArraysByMe=0;
        bool break_possible = false;
        std::array<int, ARRSIZE> queue_element={};
        while(true){
            std::unique_lock<std::mutex> lck(locker);
            if (!queue.get()->queue_vector.empty()) {
                queue_element = queue.get()->queue_vector.front();
                queue.get()->queue_vector.erase(queue.get()->queue_vector.begin());
                lck.unlock();
                NumberOfConsumedArrays++;
                NumberOfConsumedArraysByMe++;
                std::sort(queue_element.begin(), queue_element.end());
            }
            else {
                std::this_thread::yield();
                if(conditionVariable.wait_for(lck,std::chrono::seconds(1))==std::cv_status::timeout) break;
            }
        }
        std::lock_guard<std::mutex> lock(locker);
        std::cout << "I consumed: " << NumberOfConsumedArraysByMe << " arrays" << std::endl;
    }
};


int main() {
    /*8 concurrent threads are supported.*/
    /*4 Physical cores*/
    auto start=std::chrono::system_clock::now();

    auto myQueue=std::make_shared<Queue>(QUEUELENGHT);
    Producer myProducer(myQueue, NUMBEROFARRAYS);

    const std::size_t NumberOfThreads=7;
    std::array<Consumer, NumberOfThreads> myConsumers = { myQueue, myQueue, myQueue, myQueue, myQueue, myQueue, myQueue };
    std::array<std::thread, NumberOfThreads> ConsumerThread;
    std::thread ProducerThread(&Producer::adding_elements_to_queue, myProducer);
    for(int j=0; j<NumberOfThreads; j++){
        ConsumerThread[j]=std::thread(&Consumer::take_and_sort, myConsumers[j]);
    }
    ProducerThread.join();
    for (auto &Thread : ConsumerThread)
        Thread.join();
    auto stop=std::chrono::system_clock::now();
    std::chrono::duration<double> duration=stop-start;

    std::cout << "Program finished in: "<< duration.count() << std::endl;
    system("Pause");
}
