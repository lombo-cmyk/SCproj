//
// Created by lkania on 30.04.2020.
//
#include <mpi.h>
#include <cmath>
#include <iostream>
#include <unistd.h>
#include "Timer.h"
void pointToPoint(int argc, char* argv[]){
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int number = -1;
    if (rank==0) {
        number = rank;
        std::cout << "I'm sender process: " << rank << std::endl;
        for (std::size_t i = 1; i < size; i++) {
            MPI_Send(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&number,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUSES_IGNORE);
        std::cout << "I'm receiver process: " << rank << ". I received data from process " << number << std::endl ;
    }
    MPI_Finalize();
}

void pointToPoint_anySource(int argc, char* argv[]){
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int number;
    if (rank) {
        number = rank;
        std::cout << "I'm sender process: " << rank << std::endl;
            MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        for(std::size_t i = 1; i < size; i++){
            MPI_Recv(&number,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUSES_IGNORE);
            std::cout << "I'm receiver process: " << rank << ". I received data from: " << number << std::endl;
        }
    }
    MPI_Finalize();
}

void nonBlockingBarrier(int argc, char* argv[]){
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int number = -1;
    MPI_Request request;
    MPI_Status status;
    int flag;
    if (rank==0) {
        number = rank;
        sleep(2);
        for (std::size_t i = 1; i < size; i++) {
            MPI_Isend(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);
            MPI_Test(&request, &flag, &status);
            std::cout << "I'm sender process: " << rank << ". My flag is: " << flag << std::endl;
        }
    } else {
        MPI_Irecv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
        MPI_Test(&request, &flag, &status);
        std::cout << "I'm receiver process: " << rank << ". I received data from process " << number << ". My flag is: " << flag << std::endl ;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}

int is_prime(int nr)
{
    if (nr < 2)
        return 0;
    for (int i = 2; i <= sqrt(nr); i++)
    if (nr%i == 0)
        return 0;

    return 1;
}

int main (int argc, char* argv[]){
//    pointToPoint(argc, argv);
    Timer T;
    int numberToCheck = 1'000'0000;
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int iteration = rank;
    MPI_Request request;
    MPI_Status status;
    int numberOfPrimes=0;
    int totalNumberOfPrimes=0;

    if(rank==0){
        iteration = size-1;
        int something;
        while(iteration <= numberToCheck){
            MPI_Recv(&something, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            iteration++;
            MPI_Send(&iteration, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
            if(iteration >= numberToCheck){
                for(std::size_t i=1; i<size; i++){
                    MPI_Isend(&iteration, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request);
                }
            }
        }
    }
    else{
        while (iteration <= numberToCheck){
            MPI_Isend(&iteration, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
            numberOfPrimes += is_prime(iteration);
            MPI_Wait(&request, &status);
            MPI_Recv(&iteration, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        }
    }


    MPI_Reduce(&numberOfPrimes, &totalNumberOfPrimes, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank==0)
        std::cout <<"Found " << totalNumberOfPrimes << " prime numbers up to number "<< numberToCheck << std::endl;
    MPI_Finalize();
    return 0;
}

