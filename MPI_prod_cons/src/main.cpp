
#include <mpi.h>
#include <cmath>
#include <iostream>
#include "../include/Timer.h"
#include "../include/BlockingMPI.h"

int main (int argc, char* argv[]){
//    pointToPoint(argc, argv);
    Timer T;
    int number = 1500;
    std::cout << "Starting constructor" << std::endl;
    BlockingMPI PrimeChecker(argc, *&argv, number);
    std::cout << "Starting run" << std::endl;
    PrimeChecker.Run();
    PrimeChecker.showResults();
    return 0;
}
