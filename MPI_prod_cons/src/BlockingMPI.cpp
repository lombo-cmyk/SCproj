//
// Created by lkania on 23.05.2020.
//

#include "../include/BlockingMPI.h"
#include <cmath>

BlockingMPI::BlockingMPI(int argcc, char** &argvv, int number):
        argc(argcc), argv(argvv), numberToCheck(number){
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    iteration = rank;
}

BlockingMPI::~BlockingMPI() {
    MPI_Finalize();
}

void BlockingMPI::Run() {
    switch (rank) {
        case 0:
            iteration = size - 1;
            rootProducer();
            break;
        default:
            childConsumer();
    }
}

void BlockingMPI::rootProducer() {
    int something;
    while (iteration <= numberToCheck) {
        MPI_Recv(&something, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        iteration++;
        if (iteration >= numberToCheck) {
            for (std::size_t i = 1; i < size; i++)
                MPI_Isend(&iteration, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request);
        } else {
            MPI_Send(&iteration, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
        }
    }
}

void BlockingMPI::childConsumer() {
    int flag;
    while (iteration <= numberToCheck) {
        MPI_Isend(&iteration, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        numberOfPrimes += static_cast<int>(isPrime(iteration));
        MPI_Wait(&request, &status);
        MPI_Test(&request, &flag, &status);
        MPI_Recv(&iteration, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
}

void BlockingMPI::showResults() {
    MPI_Reduce(&numberOfPrimes, &totalNumberOfPrimes, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank==0)
        std::cout <<"Found " << totalNumberOfPrimes << " prime numbers up to number "<< numberToCheck << std::endl;
}

bool BlockingMPI::isPrime(int isItPrime) const{
    if(isItPrime < 2)
        return false;
    for (int i = 2; i <= sqrt(isItPrime); i++){
        if (isItPrime % i == 0)
            return false;
    }
    return true;
}
