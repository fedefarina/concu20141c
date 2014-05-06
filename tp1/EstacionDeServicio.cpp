#include "EstacionDeServicio.h"

EstacionDeServicio::EstacionDeServicio(unsigned int empleados,unsigned int surtidores) {
    this->caja.crear((char*)"/bin/ls", 'C');
    this->caja.escribir(0);
    Semaforo semaforoCaja((char*)SEMAFORO_CAJA,0);
    this->semaforoCaja=semaforoCaja;
    Semaforo semaforoSurtidores((char*)SEMAFORO_SURTIDOR,surtidores-1);
    this->surtidoresSemaforo=semaforoSurtidores;

}

EstacionDeServicio::~EstacionDeServicio() {
    this->surtidoresSemaforo.eliminar();
    this->caja.liberar();
    this->semaforoCaja.eliminar();
}


