//
// Created by lkania on 23.05.2020.
//

#include "../include/BlockingMPI.h"
#include <cmath>
#include <random>
#include <algorithm>
BlockingMPI::BlockingMPI(int argcc, char** &argvv, int number):
        argc(argcc), argv(argvv), noArrays(number){
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
            iteration = 0;//size - 1;
            rootProducer();
            break;
        default:
            childConsumer();
    }
}

void BlockingMPI::rootProducer() {
    int something;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(m_minToDraw, m_maxToDraw);
    intArray queueElement ={};
//    std::cout <<"PROD: while starting" << std::endl;
    while (iteration <= noArrays) {
        FillArray(queueElement, dis, gen);
//        std::cout <<"PROD: array filled" << std::endl;
        MPI_Recv(&something, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
//        std::cout <<"PROD: iteration received" << std::endl;
        iteration++;
        std::cout << "sending itereation: " << iteration << std::endl;
        if (iteration > noArrays) {
            for (std::size_t i = 1; i < size; i++)
                MPI_Send(&iteration, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            //break;
        } else {
            MPI_Send(&iteration, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
            MPI_Send(&queueElement, arrSize, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
        }
    }
    std::cout <<"Producer done" << std::endl;
}

void BlockingMPI::FillArray(intArray& qElement, std::uniform_int_distribution<> dis, std::mt19937& gen) const {
    for (int i = 0; i < arrSize; i++) {
        qElement[i] = dis(gen);
    }
}

void BlockingMPI::childConsumer() {
    int flag;
    intArray queueElement ={};
    int noConsumed = 0;
    std::cout <<"CONSUMER: while starting" << std::endl;
    while (iteration <= noArrays) {
        MPI_Send(&iteration, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&iteration, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        if (iteration > noArrays){
            break;
        }
        MPI_Recv(&queueElement, arrSize, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        std::sort(queueElement.begin(), queueElement.end());
//        for (auto& q:queueElement){
//            std::cout << q << " ";
//        }
        std::cout << std::endl;

        noConsumed++;
        std::cout <<"No consumed arrays:" << noConsumed << iteration << std::endl;
    }
    std::cout <<"Consumer done, no consumed:" << noConsumed << std::endl;

}

void BlockingMPI::showResults() {
    //MPI_Reduce(&numberOfPrimes, &totalNumberOfPrimes, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
//    if(rank==0)
//        std::cout << "Found " << totalNumberOfPrimes << " prime numbers up to number " << noArrays << std::endl;
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
