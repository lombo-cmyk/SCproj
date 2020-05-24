//
// Created by lkania on 24.05.2020.
//

#ifndef MPI_PROD_CONS_CONSUMER_H
#define MPI_PROD_CONS_CONSUMER_H

#include "interfaceMPI.h"
#include "types.h"
class Consumer : public interfaceMPI {
public:
    Consumer();
    ~Consumer() override;
protected:
    void childConsumer();
private:
    double CalculateAverage(intArray& passedArray) const;
};


#endif //MPI_PROD_CONS_CONSUMER_H
