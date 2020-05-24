//
// Created by lkania on 23.05.2020.
//

#include "../include/interfaceMPI.h"


interfaceMPI::interfaceMPI(int argcc, char** &argvv):
        argc(argcc), argv(argvv){
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
}

int interfaceMPI::getRank() {return rank;}

void interfaceMPI::showResults() {
    MPI_Reduce(&noConsumed, &totalNumberOfConsumed, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank==0)
        std::cout << "Found " << totalNumberOfConsumed << " arrays consumed " << std::endl;
}




