
#include <mpi.h>
#include <cmath>
#include <iostream>
#include "../include/Timer.h"
#include "../include/BlockingMPI.h"

int main (int argc, char* argv[]){
//    pointToPoint(argc, argv);
    Timer T;
    int number = 10'000'000;
    BlockingMPI PrimeChecker(argc, *&argv, number);
    PrimeChecker.Run();
    PrimeChecker.showResults();
    return 0;
}
