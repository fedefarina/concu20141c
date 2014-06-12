#include "EstacionDeServicio.h"

EstacionDeServicio* EstacionDeServicio::instancia = NULL;

EstacionDeServicio::EstacionDeServicio() {
    this->caja.crear((char*)MEMORIA_CAJA, 'C');
    this->caja.escribir(0);
    Semaforo semaforoCaja((char*)SEMAFORO_CAJA,1,1);
    Semaforo semaforoCola ( (char*)SEMAFORO_COLA,1,1);

    this->semaforoCola=semaforoCola;
    std::cout<<"estacion p value"<<this->semaforoCola.getValue()<<std::endl;
    this->semaforoCaja=semaforoCaja;

    Cola<mensaje> *cola=new Cola<mensaje>( COLA_MENSAJES,'C');
    this->colaAutos=cola;
}

EstacionDeServicio::~EstacionDeServicio() {
    this->colaAutos->destruir();
    this->semaforoSurtidores.eliminar();
    this->semaforoEmpleados.eliminar();
    this->semaforoCaja.eliminar();
    this->semaforoCola.eliminar();
    this->caja.liberar();
    this->empleados.liberar();
    this->surtidores.liberar();
    colaAutos->destruir();
    delete(this->colaAutos);
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
