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
std::condition_variable condition_producer;
std::mutex locker;
std::mutex locker_producer;
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
    static bool is_finished;
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
                //std::cout << "Producent pracuje" << std::endl;
                {
                    std::lock_guard<std::mutex> lock(locker);
                    queue.get()->queue_vector.push_back(queue_element);
                }
                //locker.unlock();
                //std::cout << "Producent dodaje element" << std::endl;
                conditionVariable.notify_one();
            }
            else{
                conditionVariable.notify_one();
                //std::lock_guard<std::mutex> lock(locker);
                //std::cout << "PRODUCER queue full" << std::endl;
                //locker.unlock();
                std::this_thread::yield();
            }

        }
        //conditionVariable.notify_one();
        std::cout << "PRODUCER FINISHED" << std::endl;
        is_finished=true;
        std::size_t size = queue.get()->queue_vector.size();
        std::cout << "Kolejka aktualna to " << size << std::endl;
        while(!queue.get()->queue_vector.empty()){
            {
                std::lock_guard<std::mutex> lock(locker);
                if(!queue.get()->queue_vector.empty())
                    conditionVariable.notify_one();
                else
                    break;
            }
        }
    }
    static bool IsFinished(){return !is_finished;}
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
        //while(NumberOfConsumedArrays < NUMBEROFARRAYS){
        while(true){
                    std::unique_lock<std::mutex> lck(locker);
                    if(conditionVariable.wait_for(lck,std::chrono::seconds(1))==std::cv_status::no_timeout) {
                        if (!queue.get()->queue_vector.empty()) {
                            //std::cout << "gogogogogogo" << std::endl;
                            queue_element = queue.get()->queue_vector.front();
                            queue.get()->queue_vector.erase(queue.get()->queue_vector.begin());
                            lck.unlock();
                            int sum = 0;
                            NumberOfConsumedArrays++;
                            NumberOfConsumedArraysByMe++;
                            std::sort(queue_element.begin(), queue_element.end());
//                            std::cout << "sorted elements: ";
//                            for (auto &a : queue_element) {
//                                sum = sum + a;
//                            }
//                            std::cout << sum / ARRSIZE << std::endl;
                            //locker.unlock();
                            }
                        }
                    else {
                            std::cout << "timeout" << std::endl;
                            std::this_thread::yield();
                            if(conditionVariable.wait_for(lck,std::chrono::seconds(1))==std::cv_status::timeout) break;
                            //std::cout << "CONSUMER No elemements in queue" << std::endl;
                            //locker.unlock();
                        }
                    //} else {
                    //    break;
                    //}
                //if(Producer::is_finished){break_possible=true;}
            }

        std::lock_guard<std::mutex> lock(locker);
        std::cout << "I consumed: " << NumberOfConsumedArraysByMe << " arrays" << std::endl;
    }
};

bool Producer::is_finished=false;

int main() {
    /*8 concurrent threads are supported.*/
    /*4 Physical cores*/
    auto start=std::chrono::system_clock::now();
    auto my_queue=std::make_shared<Queue>(QUEUELENGHT);
    Producer my_producer(my_queue, NUMBEROFARRAYS);

    const uint8_t NumberOfThreads=7;
    Consumer my_consumer_1[NumberOfThreads]={my_queue,my_queue,my_queue,my_queue,my_queue,my_queue,my_queue};
    std::array<std::thread, NumberOfThreads> ConsumerThread;
    std::thread ProducerThread(&Producer::adding_elements_to_queue,my_producer);
    for(int j=0; j<NumberOfThreads; j++){
        ConsumerThread[j]=std::thread(&Consumer::take_and_sort, my_consumer_1[j]);
    }
    ProducerThread.join();
    for (auto &Thread:ConsumerThread)
        Thread.join();
    auto stop=std::chrono::system_clock::now();
    std::chrono::duration<double> duration=stop-start;

    std::cout << "Program finished in: "<< duration.count() << std::endl;
    system("Pause");
}
