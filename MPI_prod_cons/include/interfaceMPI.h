//
// Created by lkania on 23.05.2020.
//

#ifndef MPI_PROD_CONS_INTERFACEMPI_H
#define MPI_PROD_CONS_INTERFACEMPI_H

#include <random>
#include "mpi.h"

class interfaceMPI {
public:
    static const int arrSize = 100000;
    interfaceMPI() = default;
    interfaceMPI(int argcc, char**& argvv);
    virtual ~interfaceMPI() = default;
    interfaceMPI(const interfaceMPI& a) = delete;
    interfaceMPI& operator=(interfaceMPI a) = delete;
    void showResults();
    int getRank();

protected:
    int argc;
    char** argv;
    int rank, size;
    MPI_Status status;
    int noConsumed=0,totalNumberOfConsumed=0;
};



#endif //MPI_PROD_CONS_INTERFACEMPI_H
