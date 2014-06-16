#ifndef AUTO_H
#define AUTO_H

#include <iostream>
#include <stdlib.h>
#include <time.h>

class Auto {

private:
    float capacidad;
    unsigned int tipo;
    bool atendido;

public:
    Auto() {
        srand(time(NULL));
        this->capacidad = rand() % 10 + 10;
        this->atendido=false;
    }

    float getCapacidad() {
        return this->capacidad;
    }

    void setCapacidad(float capacidad) {
        this->capacidad = capacidad;
    }

    unsigned int getTipo()const{
        return tipo;
    }

    void setTipo(unsigned int value){
        tipo = value;
    }

    void setAtendido(bool isAtendido){
        atendido=isAtendido;
    }

    bool isAtendido() const{
        return atendido;
    }

    ~Auto() {

    }
};

#endif // AUTO_H





