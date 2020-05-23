//
// Created by lkania on 23.05.2020.
//

#ifndef MPI_PROD_CONS_BLOCKINGMPI_H
#define MPI_PROD_CONS_BLOCKINGMPI_H

#include <random>
#include "mpi.h"
#include "types.h"

class BlockingMPI {
public:
    static const int arrSize = 1000000;
    BlockingMPI(int argcc, char**& argvv, int number);
    ~BlockingMPI();
    BlockingMPI(const BlockingMPI& a) = delete;
    BlockingMPI& operator=(BlockingMPI a) = delete;
    void Run();
    void showResults();

private:
    int argc;
    char** argv;
    int rank, size;
    MPI_Request request;
    MPI_Status status;
    int noArrays;
    int iteration;
    int numberOfPrimes=0,totalNumberOfPrimes=0;
    const std::size_t m_maxToDraw = 50000;
    const std::size_t m_minToDraw = -50000;
    bool isPrime(int isItPrime) const;
    void rootProducer();
    void childConsumer();
    void FillArray(intArray& qElement, std::uniform_int_distribution<> dis, std::mt19937& gen) const;
};



#endif //MPI_PROD_CONS_BLOCKINGMPI_H
