//
// Created by lkania on 24.05.2020.
//

#include "../include/Consumer.h"
#include <algorithm>

Consumer::Consumer() {
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Consumer::childConsumer();
}

Consumer::~Consumer(){
    MPI_Finalize();
}

void Consumer::childConsumer() {
    intArray queueElement ={};
    double average = 0;
    bool areIterationsDone = false;
    bool needArray = true;
    while (true) {
        MPI_Send(&needArray, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&areIterationsDone, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        if (areIterationsDone) break;
        MPI_Recv(&queueElement, arrSize, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        std::sort(queueElement.begin(), queueElement.end());
        average = CalculateAverage(queueElement);
        noConsumed++;
        std::cout <<"Process: " << rank << ", Average value: " << average << std::endl;
    }
    std::cout <<"Process: " << rank << ", NoConsumed: " << noConsumed << std::endl;
}

double Consumer::CalculateAverage(intArray& passedArray) const {
    int sum = std::accumulate(passedArray.begin(), passedArray.end(), 0);
    return static_cast<double>(sum)/passedArray.size();
}
