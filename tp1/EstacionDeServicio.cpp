#include "EstacionDeServicio.h"

EstacionDeServicio* EstacionDeServicio::instancia = NULL;

EstacionDeServicio::EstacionDeServicio() {

    Semaforo semaforoCaja((char*)SEMAFORO_CAJA,1,1);
    this->semaforoCaja=semaforoCaja;

    Semaforo semaforoCajaOcupada((char*)SEMAFORO_CAJA_OCUPADA);
    this->semaforoCajaOcupada=semaforoCajaOcupada;

    this->caja = new Caja();
    caja->setMonto(0);

    ColaPrioridad<mensaje> *colaAutos=new ColaPrioridad<mensaje>( COLA_AUTOS,'C');
    this->colaAutos=colaAutos;

    ColaPrioridad<mensaje> *colaCaja=new ColaPrioridad<mensaje>( COLA_CAJA,'C');
    this->colaCaja=colaCaja;
}

EstacionDeServicio::~EstacionDeServicio() {
    this->colaAutos->destruir();
    this->colaCaja->destruir();
    this->semaforoCaja.eliminar();
    semaforoCajaOcupada.eliminar();
    this->semaforoSurtidores.eliminar();
    this->semaforoSurtidoresDisponibles.eliminar();
    this->semaforoEmpleados.eliminar();
    this->empleados.liberar();
    this->surtidores.liberar();
    delete(this->colaAutos);
    delete(this->colaCaja);
    delete(caja);
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

    Semaforo semaforoSurtidoresDisponibles((char*)SEMAFORO_SURTIDORES_DISPONIBLES, surtidores,1);
    this->semaforoSurtidoresDisponibles=semaforoSurtidoresDisponibles;
}

unsigned int EstacionDeServicio::getEmpleados() {
    return empleados.cantidad();
}

unsigned int EstacionDeServicio::getSurtidores() {
    return surtidores.cantidad();
}
