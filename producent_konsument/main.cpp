#include <iostream>
#include <array>
#include <vector>
#include <random>
#include <memory>
#include <algorithm>
#include <mutex>
#include <thread>
#include <atomic>

#define ARRSIZE 20
#define QUEUELENGHT 20
#define NUMBEROFARRAYS 40

std::mutex locker;
std::atomic_int NumberOfConsumedArrays;
class Queue{
public:
    int q_lenght;
    std::vector<std::array<int, ARRSIZE>> queue_vector;
    //std::vector<int> queue_vector;
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
        int j=0;
        while(j<number_of_arrays) {
            if(queue.get()->queue_vector.size()<QUEUELENGHT){
                j++;
                std::uniform_int_distribution<> dis(-50000, 50000);
                std::array<int, ARRSIZE> queue_element;
                for (int i = 0; i < ARRSIZE; i++) {
                    queue_element[i] = dis(gen);
                }
                std::lock_guard<std::mutex> lock(locker);
                queue.get()->queue_vector.push_back(queue_element);
            }
            else{
                //std::lock_guard<std::mutex> lock(locker);
                //std::cout << "PRODUCER queue full" << std::endl;
                std::this_thread::yield();
            }
        }
        is_finished=true;
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
        //while(NumberOfConsumedArrays < NUMBEROFARRAYS){
        while(true){
                if(queue.get()->queue_vector.empty()!=true){
                    NumberOfConsumedArrays++;
                    NumberOfConsumedArraysByMe++;
                    locker.lock();
                    auto queue_element = queue.get()->queue_vector.front();
                    queue.get()->queue_vector.erase(queue.get()->queue_vector.begin());
                    locker.unlock();
                    std::sort(queue_element.begin(),queue_element.end());
                    locker.lock();
                    std::cout << "sorted elements: ";
                    for (auto &a:queue_element){
                        std::cout << a << " ";
                    }
                    std::cout << std::endl;
                    locker.unlock();
                }
                else{
                    std::this_thread::yield();
                    //std::lock_guard<std::mutex> lock(locker);
                    //std::cout << "CONSUMER No elemements in queue" << std::endl;
                    //std::this_thread::sleep_for(std::chrono::microseconds(100));
                }
                if(Producer::is_finished){
                    break;
                }
            }

        std::lock_guard<std::mutex> lock(locker);
        std::cout << "I consumed: " << NumberOfConsumedArraysByMe << " arrays" << std::endl;
    }
};

bool Producer::is_finished=false;

int main() {

    auto my_queue=std::make_shared<Queue>(QUEUELENGHT);
    Producer my_producer(my_queue, NUMBEROFARRAYS);
    Consumer my_consumer_1(my_queue);
    Consumer my_consumer_2(my_queue);
    Consumer my_consumer_3(my_queue);
    std::thread ProducerThread(&Producer::adding_elements_to_queue,my_producer);
    std::thread ConsumerThread_1(&Consumer::take_and_sort, my_consumer_1);
    std::thread ConsumerThread_2(&Consumer::take_and_sort, my_consumer_2);
    std::thread ConsumerThread_3(&Consumer::take_and_sort, my_consumer_3);
//    std::thread ConsumerThread_4(&Consumer::take_and_sort, my_consumer);
//    std::thread ConsumerThread_6(&Consumer::take_and_sort, my_consumer);
//    std::thread ConsumerThread_5(&Consumer::take_and_sort, my_consumer);
    ProducerThread.join();
    ConsumerThread_1.join();
    ConsumerThread_2.join();
    ConsumerThread_3.join();
    std::cout << "Program finished" << std::endl;

}
