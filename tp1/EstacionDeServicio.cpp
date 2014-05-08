#include "EstacionDeServicio.h"

EstacionDeServicio::EstacionDeServicio(unsigned int empleados,unsigned int surtidores) {
    this->caja.crear((char*)"/bin/ls", 'C');
    this->caja.escribir(0);
    Semaforo semaforoCaja((char*)SEMAFORO_CAJA,1);
    Semaforo semaforoSurtidores((char*)SEMAFORO_SURTIDOR,surtidores);
    this->semaforoCaja=semaforoCaja;
    this->surtidoresSemaforo=semaforoSurtidores;
}

EstacionDeServicio::~EstacionDeServicio() {
    this->surtidoresSemaforo.eliminar();
    this->semaforoCaja.eliminar();
    this->caja.liberar();
}


