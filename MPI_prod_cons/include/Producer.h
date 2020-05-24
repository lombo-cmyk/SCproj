//
// Created by lkania on 24.05.2020.
//

#ifndef MPI_PROD_CONS_PRODUCER_H
#define MPI_PROD_CONS_PRODUCER_H

#include "interfaceMPI.h"
#include "types.h"
class Producer : public interfaceMPI{
public:
    Producer(int number);
    ~Producer() override;
protected:
    void rootProducer();
private:
    int noArrays;
    int something;
    int flag;
    int drawIteration = 0;
    int queueLength = 200;
    int iteration=0;
    bool isReceiving = true;
    bool areIterationsDone = false;
    std::vector<std::array<int, arrSize>> queueVector;
    const std::size_t m_maxToDraw = 50000;
    const std::size_t m_minToDraw = -50000;
    MPI_Request request, request_iter, request_arr;
    std::random_device rd;
    void fillArray(intArray& qElement, std::uniform_int_distribution<> dis, std::mt19937& gen) const;
    void sendFinishNotification();
    void sendQueueElement();
};


#endif //MPI_PROD_CONS_PRODUCER_H
