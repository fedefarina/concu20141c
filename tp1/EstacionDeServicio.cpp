#include "EstacionDeServicio.h"

#define SEMAFORO_SURTIDOR "EstacionDeServicio.h"
#define SEMAFORO_CAJA "Empleado.h"

EstacionDeServicio EstacionDeServicio::instance;

EstacionDeServicio::EstacionDeServicio() {
    this->caja.crear((char*)"/bin/ls", 'C');
    this->caja.escribir(0);
    Semaforo semaforoCaja((char*)SEMAFORO_CAJA,0);
    this->semaforoCaja=semaforoCaja;
}



EstacionDeServicio& EstacionDeServicio::getInstance() {
   return instance;
}

void EstacionDeServicio::setEmpleados(unsigned int empleados) {
    this->empleados=empleados;
}

void EstacionDeServicio::setSurtidores(unsigned int surtidores) {
    this->surtidores=surtidores;
    Semaforo semaforoSurtidores((char*)SEMAFORO_SURTIDOR,surtidores-1);
    this->surtidoresSemaforo=semaforoSurtidores;
}

void EstacionDeServicio::setJefeDeEstacion(const JefeDeEstacion& jefeEstacion){
    this->jefeDeEstacion=jefeEstacion;
}

void EstacionDeServicio::guardarEnCaja(unsigned int monto){
    this->semaforoCaja.p();
    unsigned int saldo=this->caja.leer();
    this->caja.escribir(saldo+monto);
    this->semaforoCaja.v();
}

JefeDeEstacion EstacionDeServicio::getJefeDeEstacion() const{
    return this->jefeDeEstacion;
}

void EstacionDeServicio::usarSurtidor(){
    this->surtidoresSemaforo.p();
}

void EstacionDeServicio::liberarSurtidor(){
    this->surtidoresSemaforo.v();
}

unsigned int EstacionDeServicio::getEmpleados() const{
    return this->empleados;
}


EstacionDeServicio::~EstacionDeServicio() {
    this->surtidoresSemaforo.eliminar();
    this->caja.liberar();
    this->semaforoCaja.eliminar();
}


