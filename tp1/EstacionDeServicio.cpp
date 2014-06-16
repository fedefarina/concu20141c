#include "EstacionDeServicio.h"

EstacionDeServicio* EstacionDeServicio::instancia = NULL;

EstacionDeServicio::EstacionDeServicio() {

    Semaforo semaforoCaja((char*)SEMAFORO_CAJA,1,1);
    this->semaforoCaja=semaforoCaja;

    Semaforo semaforoColaAutos ( (char*)SEMAFORO_COLA_AUTOS,1,1);
    this->semaforoColaAutos=semaforoColaAutos;

    Semaforo semaforoColaCaja( (char*)SEMAFORO_COLA_CAJA,1,1);
    this->semaforoColaCaja=semaforoColaCaja;

    Semaforo semaforoContadorVIP((char*) SEMAFORO_CONTADOR_VIP,1,1);
    this->semaforoContadorVIP=semaforoContadorVIP;

    this->contadorVIP.crear((char*) MEMORIA_CONTADOR_VIP, 'M');
    contadorVIP.escribir(0);

    this->caja = new Caja();
    caja->depositarMonto(0,false);

    Cola<mensaje> *colaAutos=new Cola<mensaje>( COLA_AUTOS,'C');
    this->colaAutos=colaAutos;

    Cola<mensaje> *colaCaja=new Cola<mensaje>( COLA_CAJA,'C');
    this->colaCaja=colaCaja;
}

EstacionDeServicio::~EstacionDeServicio() {
    this->colaAutos->destruir();
    this->colaCaja->destruir();
    this->semaforoSurtidores.eliminar();
    this->semaforoContadorVIP.eliminar();
    this->semaforoEmpleados.eliminar();
    this->semaforoColaAutos.eliminar();
    this->semaforoColaCaja.eliminar();
    this->semaforoCaja.eliminar();
    this->empleados.liberar();
    this->surtidores.liberar();
    this->contadorVIP.liberar();
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
}

unsigned int EstacionDeServicio::getEmpleados() {
    return empleados.cantidad();
}

unsigned int EstacionDeServicio::getSurtidores() {
    return surtidores.cantidad();
}
