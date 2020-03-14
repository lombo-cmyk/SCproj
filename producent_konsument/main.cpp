#include <iostream>
#include <array>
#include <vector>
#include <random>
#include <memory>
#include <algorithm>
#include <mutex>

#define ARRSIZE 100
#define QUEUELENGHT 10
std::mutex locker;

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
    int number_of_elements;

    Producer(std::shared_ptr<Queue> passed_queue, int passed_number_of_elements){
        queue=passed_queue;
        number_of_elements=passed_number_of_elements;
    }
    void adding_elements_to_queue(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(-50000, 50000);
        std::array<int, ARRSIZE> queue_element;
        for (int i=0; i<number_of_elements; i++){
            queue_element[i]=dis(gen);
        }
        std::lock_guard<std::mutex> lock(locker);
        queue.get()->queue_vector.push_back(queue_element);
    }
};

class Consumer{
public:
    std::shared_ptr<Queue> queue;
    Consumer(std::shared_ptr<Queue> passed_queue){
     queue=passed_queue;
    }
    void take_and_sort(){
        locker.lock();
        auto queue_element = queue.get()->queue_vector.front();
        queue.get()->queue_vector.erase(queue.get()->queue_vector.begin());
        locker.unlock();
        std::sort(queue_element.begin(),queue_element.end());
        for (auto &a:queue_element){
            std::cout << "sorted elements: " << a << std::endl;
        }
    }
};



int main() {

    auto my_queue=std::make_shared<Queue>(QUEUELENGHT);
    Producer my_producer(my_queue, ARRSIZE);
    my_producer.adding_elements_to_queue();
    Consumer my_consumer(my_queue);
    my_consumer.take_and_sort();
}
