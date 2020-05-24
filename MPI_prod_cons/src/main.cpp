
#include "../include/Timer.h"
#include "../include/interfaceMPI.h"
#include "../include/Producer.h"
#include "../include/Consumer.h"


int main (int argc, char* argv[]){
    Timer T;
    int numberOfArrays = 1500;
    interfaceMPI PrimeChecker(argc, *&argv);
    int rank = PrimeChecker.getRank();
    if (rank == 0){
        Producer myProducer(numberOfArrays);
        myProducer.showResults();
    }
    else{
        Consumer myConsumer;
        myConsumer.showResults();
    }
    return 0;
}
