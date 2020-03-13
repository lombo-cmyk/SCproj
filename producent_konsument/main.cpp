#include <iostream>
#include <array>
#include <vector>
#include <random>
#include <memory>

#define ARRSIZE 10
#define QUEUELENGHT 10

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
        queue.get()->queue_vector.push_back(queue_element);
    }
};



int main() {

    auto my_queue=std::make_shared<Queue>(QUEUELENGHT);
    Producer my_producer(my_queue, 10);
    my_producer.adding_elements_to_queue();
    std::array<int, ARRSIZE> queue_element{};
    for(int i=0; i<10; i++){
        std::cout << my_queue.get()->queue_vector[0][i] << std::endl;
    }

}
