//
// Created by lkania on 02.05.2020.
//

#ifndef MPIBASICS_MYMPI_H
#define MPIBASICS_MYMPI_H

#include "mpi.h"
class MyMpi{
public:
    MyMpi(int argcc, char* argvv[]);//, int number);
    ~MyMpi();
    MyMpi(const MyMpi& a) = delete;
    MyMpi& operator=(MyMpi a) = delete;
    void Run();
    void showResults();

private:
    int argc;
    char** argv;
    int rank, size;
    MPI_Request request;
    MPI_Status status;
    int numberToCheck=1'000'000;
    int iteration;
    int numberOfPrimes=0,totalNumberOfPrimes=0;
    bool isPrime(int isItPrime) const;
    void rootSender();
    void childPrimeHunter();
};








#endif //MPIBASICS_MYMPI_H
