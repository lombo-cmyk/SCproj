#include <iostream>
#include <array>
#include <vector>
#include <random>


#define ARRSIZE 100000

class Queue{
public:
    int q_lenght;
    static std::vector<std::array<int, ARRSIZE>> queue_vector;
    //std::vector<int> queue_vector;
    Queue(int queue_lenght){
        q_lenght=queue_lenght;
    }
//    void add_element(std::array<int, ARRSIZE> new_element){
//        if(queue_vector.size()<q_lenght){
//            queue_vector.push_back(new_element);
//        }
//    }
//
//    std::array<int, ARRSIZE> take_element(){
//        std::array<int, ARRSIZE> ret_val=queue_vector.front();
//        queue_vector.erase(queue_vector.begin());
//        return ret_val;
//    }

};

class Producer{
public:
    std::vector<std::array<int, ARRSIZE>> queue;
    int number_of_arrays;

    Producer(std::vector<std::array<int, ARRSIZE>> passed_queue, int passed_number_of_arrays){
        queue=passed_queue;
        number_of_arrays=passed_number_of_arrays;
    }
    void adding_elements_to_queue(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 50000);
        std::array<int, ARRSIZE> queue_element;
        for (int i=0; i<number_of_arrays; i++){
            queue_element[i]=dis(gen);
        }
        queue.push_back(queue_element);
        Queue::queue_vector.push_back(queue_element);
    }
};



int main() {

    Queue my_queue(10);
    Producer my_producer(Queue::queue_vector, 1000);


    std::array<int, ARRSIZE> queue_element{};
    queue_element[0]=1;
    queue_element[1]=2;
    queue_element[2]=3;
    std::cout << queue_element[0] << queue_element[1] << queue_element[2] << std::endl;

}
