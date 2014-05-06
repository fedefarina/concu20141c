#ifndef ESTACIONDESERVICIO_H
#define ESTACIONDESERVICIO_H

#include <iostream>
#include "Semaforo.h"
#include "JefeDeEstacion.h"

using namespace std;

class EstacionDeServicio {

private:
    static EstacionDeServicio instance;
    MemoriaCompartida<float> caja;
    Semaforo surtidoresSemaforo;
    Semaforo semaforoCaja;

    unsigned int empleados;
    unsigned int surtidores;
    EstacionDeServicio();

public:    
    JefeDeEstacion jefeDeEstacion;
    static EstacionDeServicio& getInstance();
    void setSurtidores(unsigned int surtidores);
    void setEmpleados(unsigned int empleados);
    void setJefeDeEstacion(const JefeDeEstacion& jefeEstacion);
    void guardarEnCaja(unsigned int monto);
    void liberarSurtidor();
    void usarSurtidor();
    JefeDeEstacion getJefeDeEstacion() const;
    unsigned int getEmpleados() const;
    ~EstacionDeServicio();

};

#endif // ESTACIONDESERVICIO_H
