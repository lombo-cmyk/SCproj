//
// Created by lkania on 23.05.2020.
//

#ifndef MPI_PROD_CONS_PRODUCERCONSUMERMPI_H
#define MPI_PROD_CONS_PRODUCERCONSUMERMPI_H

#include <random>
#include "mpi.h"
#include "types.h"

class ProducerConsumerMPI {
public:
    static const int arrSize = 100000;
    ProducerConsumerMPI(int argcc, char**& argvv, int number);
    ~ProducerConsumerMPI();
    ProducerConsumerMPI(const ProducerConsumerMPI& a) = delete;
    ProducerConsumerMPI& operator=(ProducerConsumerMPI a) = delete;
    void Run();
    void showResults();

private:
    int argc;
    char** argv;
    int rank, size;
    MPI_Request request, request_iter, request_arr;
    MPI_Status status;
    int noArrays;
    int iteration;
    int noConsumed=0,totalNumberOfConsumed=0;
    const std::size_t m_maxToDraw = 50000;
    const std::size_t m_minToDraw = -50000;
    void rootProducer();
    void childConsumer();
    void FillArray(intArray& qElement, std::uniform_int_distribution<> dis, std::mt19937& gen) const;
    double CalculateAverage(intArray& passedArray) const;
};



#endif //MPI_PROD_CONS_PRODUCERCONSUMERMPI_H
