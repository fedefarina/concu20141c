#include "EstacionDeServicio.h"

EstacionDeServicio* EstacionDeServicio::instancia = NULL;

EstacionDeServicio::EstacionDeServicio() {
    this->caja.crear((char*)MEMORIA_CAJA, 'C');
    this->caja.escribir(0);
    Semaforo semaforoCaja((char*)SEMAFORO_CAJA,1,1);
    Semaforo semaforoFifo ( (char*)SEMAFORO_FIFO,0,1);
    this->semaforoFifo=semaforoFifo;
    this->semaforoCaja=semaforoCaja;
}

EstacionDeServicio::~EstacionDeServicio() {
    this->semaforoSurtidores.eliminar();
    this->semaforoEmpleados.eliminar();
    this->semaforoFifo.eliminar();
    this->semaforoCaja.eliminar();
    this->caja.liberar();
    this->empleados.liberar();
    this->surtidores.liberar();
}

EstacionDeServicio *EstacionDeServicio::getInstancia() {
    if (instancia == NULL)
        instancia = new EstacionDeServicio();
    return instancia;
}

void EstacionDeServicio::destruirInstancia() {
    if (instancia != NULL){
        delete (instancia);
        instancia=NULL;
    }
}

void EstacionDeServicio::setEmpleados(unsigned int empleados) {
    this->empleados.crear((char*) MEMORIA_EMPLEADOS, 'E', empleados);
    Semaforo semaforoEmpleados((char*)SEMAFORO_EMPLEADOS, 1, empleados);
    this->semaforoEmpleados = semaforoEmpleados;
    for (unsigned int i = 0; i < empleados; i++)
        this->empleados.escribir(true, i);
}

void EstacionDeServicio::setSurtidores(unsigned int surtidores) {
    this->surtidores.crear((char*)MEMORIA_SURTIDORES, 'S', surtidores );
    for (unsigned int i = 0; i < surtidores; i++)
        this->surtidores.escribir(true, i);

    Semaforo semaforoSurtidores((char*)SEMAFORO_SURTIDOR, 1, surtidores);
    this->semaforoSurtidores=semaforoSurtidores;
}

unsigned int EstacionDeServicio::getEmpleados() {
    return empleados.cantidad();
}

unsigned int EstacionDeServicio::getSurtidores() {
    return surtidores.cantidad();
}
