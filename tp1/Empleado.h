#ifndef EMPLEADO_H
#define EMPLEADO_H

#include "EstacionDeServicio.h"
#include "Auto.h"
#include "Constantes.h"
#include "MemoriaCompartida.h"
#include "Logger.h"
#include "Semaforo.h"
#include "Caja.h"
#include "mainwindow.h"

class Empleado {

private:    
    MemoriaCompartida<bool> surtidores;
    Caja* caja;
    Cola<mensaje>* colaAutos;
    Semaforo semaforoSurtidores;
    Semaforo semaforoColaAutos;

public:

    Empleado() {
        unsigned int surtidores = EstacionDeServicio::getInstancia()->getSurtidores();

        this->surtidores.crear((char*)MEMORIA_SURTIDORES, 'S', surtidores);

        Semaforo semaforoSurtidores((char*)SEMAFORO_SURTIDOR);
        this->semaforoSurtidores=semaforoSurtidores;

        Semaforo semaforoCola((char*) SEMAFORO_COLA_AUTOS);
        this->semaforoColaAutos=semaforoCola;

        Cola<mensaje> *colaAutos= new Cola<mensaje>( COLA_AUTOS,'C');
        this->colaAutos=colaAutos;
        this->caja = new Caja();
    }

    void atenderAuto(Auto& unAuto) {
        float tiempoDeCarga = unAuto.getCapacidad();
        for (unsigned int i = 0; i < surtidores.cantidad(); i++) {
            semaforoSurtidores.p(i);
            if (this->surtidores.leer(i) == true) {
                this->surtidores.escribir(false, i);
                semaforoSurtidores.v(i);

                string tipo=(unAuto.getTipo()==AUTO_VIP?" VIP":"");
                Logger::debug(getpid(), "Evento -> Atendiendo auto"+tipo+"\n");
                Utils<int> utils;
                Logger::debug(getpid(),"Usando surtidor "  + utils.toString(i+1) +"\n");
                sleep(tiempoDeCarga);

                caja->depositarMonto(tiempoDeCarga);
                Logger::debug(getpid(),"Saldo de caja: "  + utils.toString(caja->getSaldo()) +"\n");
                Logger::debug(getpid(), "Auto" + tipo + " atendido\n");
                semaforoSurtidores.p(i);
                this->surtidores.escribir(true, i);
                semaforoSurtidores.v(i);
                unAuto.setAtendido(true);
                return;
            }
            semaforoSurtidores.v(i);
        }
    }

    ~Empleado() {
        this->surtidores.liberar();
        delete(this->colaAutos);
        delete(this->caja);
    }

};

#endif // EMPLEADO_H
