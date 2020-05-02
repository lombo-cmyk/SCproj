//
// Created by lkania on 30.04.2020.
//
#include <mpi.h>
#include <cmath>
#include <iostream>
#include "../include/Timer.h"
#include "../include/MyMpi.h"

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


int main (int argc, char* argv[]){
//    pointToPoint(argc, argv);
    Timer T;
    int number = 1'000'000;
    MyMpi PrimeChecker(argc, *&argv, number);
    PrimeChecker.Run();
    PrimeChecker.showResults();
    return 0;
}

