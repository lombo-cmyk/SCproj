//
// Created by lkania on 24.05.2020.
//

#include "../include/Producer.h"
Producer::Producer(int number):noArrays(number){
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Producer::rootProducer();
}

Producer::~Producer() {
    MPI_Finalize();
}

void Producer::rootProducer() {
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(m_minToDraw, m_maxToDraw);
    intArray queueElement ={};
    while (iteration <= noArrays) {
        if (drawIteration < noArrays && queueVector.size() < queueLength){
            fillArray(queueElement, dis, gen);
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
                sendFinishNotification();
            } else {
                sendQueueElement();
            }
            isReceiving = true;
        }
    }
    std::cout << "Producer done, produced: " << iteration-1 << " arrays."  << std::endl;
}

void Producer::fillArray(intArray& qElement, std::uniform_int_distribution<> dis, std::mt19937& gen) const {
    for (int i = 0; i < arrSize; i++) {
        qElement[i] = dis(gen);
    }
}

void Producer::sendFinishNotification() {
    areIterationsDone = true;
    for (std::size_t i = 1; i < size; i++)
        MPI_Isend(&areIterationsDone, 1, MPI_C_BOOL, i, 0, MPI_COMM_WORLD, &request_iter);
}

void Producer::sendQueueElement() {
    MPI_Isend(&areIterationsDone, 1, MPI_C_BOOL, status.MPI_SOURCE, 0, MPI_COMM_WORLD, &request_iter);
    MPI_Isend(&queueVector.front(), arrSize, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD, &request_arr);
    queueVector.erase(queueVector.begin());
}



