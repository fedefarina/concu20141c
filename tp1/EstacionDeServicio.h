#ifndef ESTACIONDESERVICIO_H
#define ESTACIONDESERVICIO_H

#include <iostream>
#include "Semaforo.h"
#include "MemoriaCompartida.h"
#include "Constantes.h"
#include "Mensajes.h"
#include "Cola.h"
#include "Caja.h"

using namespace std;

class EstacionDeServicio {

private:
    Caja* caja;
    MemoriaCompartida<bool> empleados;
    MemoriaCompartida<bool> surtidores;
    MemoriaCompartida<unsigned int> contadorVIP;
    MemoriaCompartida<bool> cajaFinalizada;

    Semaforo semaforoSurtidores;
    Semaforo semaforoEmpleados;
    Semaforo semaforoCaja;
    Semaforo semaforoCajaOcupada;
    Semaforo semaforoCajaFinalizada;
    Semaforo semaforoColaAutos;
    Semaforo semaforoColaCaja;
    Semaforo semaforoContadorVIP;
    Cola<mensaje> *colaAutos;
    Cola<mensaje> *colaCaja;

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
