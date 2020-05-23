
#include <mpi.h>
#include <cmath>
#include <iostream>
#include "../include/Timer.h"
#include "../include/ProducerConsumerMPI.h"

int main (int argc, char* argv[]){
    Timer T;
    int number = 1500;
    std::cout << "Starting constructor" << std::endl;
    ProducerConsumerMPI PrimeChecker(argc, *&argv, number);
    std::cout << "Starting run" << std::endl;
    PrimeChecker.Run();
    PrimeChecker.showResults();
    return 0;
}
