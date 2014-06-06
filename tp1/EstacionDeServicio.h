#ifndef ESTACIONDESERVICIO_H
#define ESTACIONDESERVICIO_H

#include <iostream>
#include "Semaforo.h"
#include "JefeDeEstacion.h"

using namespace std;

class EstacionDeServicio {

private:
    MemoriaCompartida<float> caja;
    Semaforo surtidoresSemaforo;
    Semaforo semaforoCaja;
    Semaforo semaforoFifo;

public:    
    EstacionDeServicio(unsigned int surtidores);
    ~EstacionDeServicio();

};

#endif // ESTACIONDESERVICIO_H
