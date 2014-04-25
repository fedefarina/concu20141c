#ifndef ESTACIONDESERVICIO_H
#define ESTACIONDESERVICIO_H

#include <iostream>
#include "MemoriaCompartida.h"
using namespace std;

class EstacionDeServicio {

private:
    MemoriaCompartida<bool> surtidores;
    static EstacionDeServicio* instancia;

public:
    EstacionDeServicio();
    ~EstacionDeServicio();

    static EstacionDeServicio* getInstancia();
    static void destruirInstancia();
    void setSurtidores(unsigned int surtidores);
    unsigned int getSurtidores();
};

#endif // ESTACIONDESERVICIO_H
