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

bool is_prime(int isItPrime)
{
    if(isItPrime < 2)
        return false;
    for (int i = 2; i <= sqrt(isItPrime); i++){
        if (isItPrime % i == 0)
            return false;
    }
    return true;
}


int main (int argc, char* argv[]){
//    pointToPoint(argc, argv);
    Timer T;
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Request request;
    MPI_Status status;
    int numberToCheck = 1'000'000;
    int iteration = rank;
    int numberOfPrimes=0,totalNumberOfPrimes=0;

    switch (rank) {
        case 0:
            iteration = size - 1;
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
            break;
        default:
            while (iteration <= numberToCheck) {
                MPI_Isend(&iteration, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
                numberOfPrimes += static_cast<int>(is_prime(iteration));
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

