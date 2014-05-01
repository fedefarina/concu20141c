#ifndef AUTO_H
#define AUTO_H

#include <iostream>
#include <stdlib.h>
#include <time.h>

class Auto {

private:
    float capacidad;

public:
    Auto() {
        srand(time(NULL));
        this->capacidad = rand() % 10 + 10;
    }

    float getCapacidad() {
        return this->capacidad;
    }

    void setCapacidad(float capacidad) {
            this->capacidad = capacidad;
    }

    ~Auto() {

    }
};



#endif // AUTO_H
