//
// Created by lkania on 23.05.2020.
//

#include "../include/ProducerConsumerMPI.h"
#include <random>
#include <algorithm>
#include <vector>
ProducerConsumerMPI::ProducerConsumerMPI(int argcc, char** &argvv, int number):
        argc(argcc), argv(argvv), noArrays(number){
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    iteration = rank;
}

ProducerConsumerMPI::~ProducerConsumerMPI() {
    MPI_Finalize();
}

void ProducerConsumerMPI::Run() {
    switch (rank) {
        case 0:
            iteration = 0;
            rootProducer();
            break;
        default:
            childConsumer();
    }
}

void ProducerConsumerMPI::rootProducer() {
    int something;
    int flag;
    int drawIteration = 0;
    int queueLength = 3;
    bool isReceiving = true;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(m_minToDraw, m_maxToDraw);
    intArray queueElement ={};
    std::vector<std::array<int, arrSize>> queueVector;
    while (iteration <= noArrays) {
        if (drawIteration < noArrays && queueVector.size() < queueLength){
            FillArray(queueElement, dis, gen);
            drawIteration++;
            queueVector.emplace_back(queueElement);
        }
        if(isReceiving){
            MPI_Irecv(&something, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);
            isReceiving = false;
        }
        MPI_Test(&request, &flag, &status);
        if(flag ==1){
            iteration++;
            if (iteration > noArrays) {
                for (std::size_t i = 1; i < size; i++)
                    MPI_Isend(&iteration, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request_iter);
            } else {
                MPI_Isend(&iteration, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD, &request_iter);
                queueElement = queueVector.front();
                MPI_Isend(&queueElement, arrSize, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD, &request_arr);
                queueVector.erase(queueVector.begin());
            }
            isReceiving = true;
        }
    }
    std::cout << "Producer done, produced: " << iteration-1 << " arrays."  << std::endl;
}

void ProducerConsumerMPI::FillArray(intArray& qElement, std::uniform_int_distribution<> dis, std::mt19937& gen) const {
    for (int i = 0; i < arrSize; i++) {
        qElement[i] = dis(gen);
    }
}

void ProducerConsumerMPI::childConsumer() {
    intArray queueElement ={};
    double average = 0;
    while (iteration <= noArrays) {
        MPI_Send(&iteration, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&iteration, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        if (iteration > noArrays){
            break;
        }
        MPI_Recv(&queueElement, arrSize, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        std::sort(queueElement.begin(), queueElement.end());
        average = CalculateAverage(queueElement);
        noConsumed++;
//        std::cout <<"Process: " << rank << ", Average value: " << average << std::endl;
    }
    std::cout <<"Process: " << rank << ", NoConsumed: " << noConsumed << std::endl;
}

double ProducerConsumerMPI::CalculateAverage(intArray& passedArray) const {
    int sum = std::accumulate(passedArray.begin(), passedArray.end(), 0);
    return static_cast<double>(sum)/passedArray.size();
}

void ProducerConsumerMPI::showResults() {
    MPI_Reduce(&noConsumed, &totalNumberOfConsumed, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank==0)
        std::cout << "Found " << totalNumberOfConsumed << " arrays consumed " << std::endl;
}


