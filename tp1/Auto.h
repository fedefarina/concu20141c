#ifndef AUTO_H
#define AUTO_H

#include "Constantes.h"
#include <iostream>
#include <stdlib.h>

class Auto {

private:
    long capacidad;
    unsigned int tipo;

public:
    Auto() {
        this->capacidad = 0;
        this->tipo=AUTO;
    }

    float getCapacidad() {
        return (this->capacidad);
    }

    void setCapacidad(long capacidad) {
        this->capacidad = capacidad;
    }

    unsigned int getTipo()const{
        return (this->tipo);
    }

    void setTipo(unsigned int tipo){
        this->tipo = tipo;
    }

    ~Auto() {

    }
};

#endif // AUTO_H





