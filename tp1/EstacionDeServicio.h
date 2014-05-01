#ifndef ESTACIONDESERVICIO_H
#define ESTACIONDESERVICIO_H

#include <iostream>
#include "MemoriaCompartida.h"
using namespace std;

class EstacionDeServicio {

private:
    MemoriaCompartida<bool> surtidores;
    static EstacionDeServicio instance;

    EstacionDeServicio();

public:    
    ~EstacionDeServicio();

    static EstacionDeServicio& getInstance();
    //static void destruirInstancia();
    void setSurtidores(unsigned int surtidores);
    unsigned int getSurtidores();
};

#endif // ESTACIONDESERVICIO_H
