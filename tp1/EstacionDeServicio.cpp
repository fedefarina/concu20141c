#include "EstacionDeServicio.h"

EstacionDeServicio* EstacionDeServicio::instancia = NULL;

EstacionDeServicio::EstacionDeServicio() {

    Semaforo semaforoCaja((char*)SEMAFORO_CAJA,1,1);
    this->semaforoCaja=semaforoCaja;

    this->caja = new Caja();
    caja->setMonto(0);

    Cola<mensaje> *colaAutos=new Cola<mensaje>( COLA_AUTOS,'C');
    this->colaAutos=colaAutos;

    Cola<mensajeCaja> *colaCaja=new Cola<mensajeCaja>( COLA_CAJA,'C');
    this->colaCaja=colaCaja;

    Cola<mensaje> *colaSurtidores=new Cola<mensaje>( COLA_SURTIDORES,'C');
    this->colaSurtidores=colaSurtidores;
}

EstacionDeServicio::~EstacionDeServicio() {
    this->colaAutos->destruir();
    this->colaCaja->destruir();
    this->colaSurtidores->destruir();
    this->semaforoCaja.eliminar();
    this->semaforoSurtidores.eliminar();
    this->semaforoEmpleados.eliminar();
    this->empleados.liberar();
    this->surtidores.liberar();
    delete(this->colaAutos);
    delete(this->colaCaja);
    delete(this->colaSurtidores);
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

    mensaje msg;
    msg.mtype=SURTIDOR;
    for(unsigned int i=0;i<surtidores;i++)
        colaSurtidores->escribir(msg);

}

unsigned int EstacionDeServicio::getEmpleados() {
    return empleados.cantidad();
}

unsigned int EstacionDeServicio::getSurtidores() {
    return surtidores.cantidad();
}
