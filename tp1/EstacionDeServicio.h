#ifndef ESTACIONDESERVICIO_H
#define ESTACIONDESERVICIO_H

#include <iostream>
#include "Semaforo.h"
#include "MemoriaCompartida.h"
#include "Constantes.h"
#include "Mensajes.h"
#include "Cola.h"

using namespace std;

class EstacionDeServicio {

private:
    MemoriaCompartida<short int> caja;
    MemoriaCompartida<bool> empleados;
    MemoriaCompartida<bool> surtidores;

    Semaforo semaforoSurtidores;
    Semaforo semaforoEmpleados;
    Semaforo semaforoCaja;
    Semaforo semaforoCola;

    Cola<mensaje> *colaAutos;

    static EstacionDeServicio* instancia;

public:    
    EstacionDeServicio();
    ~EstacionDeServicio();

    static EstacionDeServicio* getInstancia();
    static void destruirInstancia();

    void setEmpleados(unsigned int empleados);
    void setSurtidores(unsigned int surtidores);

    unsigned int getEmpleados();
    unsigned int getSurtidores();
};

#endif // ESTACIONDESERVICIO_H
