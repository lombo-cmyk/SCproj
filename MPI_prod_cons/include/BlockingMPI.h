//
// Created by lkania on 23.05.2020.
//

#ifndef MPI_PROD_CONS_BLOCKINGMPI_H
#define MPI_PROD_CONS_BLOCKINGMPI_H

#include "mpi.h"
class BlockingMPI {
public:
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
    int numberToCheck;
    int iteration;
    int numberOfPrimes=0,totalNumberOfPrimes=0;
    bool isPrime(int isItPrime) const;
    void rootProducer();
    void childConsumer();
};



#endif //MPI_PROD_CONS_BLOCKINGMPI_H
