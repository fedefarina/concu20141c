#ifndef AUTO_H
#define AUTO_H

#include <iostream>
#include <stdlib.h>

class Auto {

private:
    float capacidad;
    unsigned int tipo;
    bool atendido;

public:
    Auto() {
        this->capacidad = 0;
        this->atendido = false;
    }

    float getCapacidad() {
        return (this->capacidad);
    }

    void setCapacidad(float capacidad) {
        this->capacidad = capacidad;
    }

    unsigned int getTipo()const{
        return (this->tipo);
    }

    void setTipo(unsigned int tipo){
        this->tipo = tipo;
    }

    void setAtendido(bool isAtendido){
        this->atendido = isAtendido;
    }

    bool isAtendido() const{
        return (this->atendido);
    }

    ~Auto() {

    }
};

#endif // AUTO_H





