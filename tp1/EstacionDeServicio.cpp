#include "EstacionDeServicio.h"

EstacionDeServicio::EstacionDeServicio(unsigned int surtidores) {

    this->caja.crear((char*)MEMORIA_CAJA, 'C');
    this->caja.escribir(0);

    Semaforo semaforoCaja((char*)SEMAFORO_CAJA,1);
    Semaforo semaforoSurtidores((char*)SEMAFORO_SURTIDOR,surtidores);
    Semaforo semaforoFifo ( (char*)SEMAFORO_FIFO,0 );

    this->semaforoFifo=semaforoFifo;
    this->semaforoCaja=semaforoCaja;
    this->surtidoresSemaforo=semaforoSurtidores;
}

void EstacionDeServicio::setJefeDeEstacion(JefeDeEstacion& jefe){
    this->jefeDeEstacion=jefe;
}

EstacionDeServicio::~EstacionDeServicio() {
    this->surtidoresSemaforo.eliminar();
    this->semaforoFifo.eliminar();
    this->semaforoCaja.eliminar();
    this->caja.liberar();
}


