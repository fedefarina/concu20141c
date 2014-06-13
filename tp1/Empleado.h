#ifndef EMPLEADO_H
#define EMPLEADO_H

#include "EstacionDeServicio.h"
#include "Auto.h"
#include "Constantes.h"
#include "MemoriaCompartida.h"
#include "Logger.h"
#include "Semaforo.h"

class Empleado {

private:    
    MemoriaCompartida<short int> caja;
    MemoriaCompartida<bool> surtidores;
    Cola<mensaje>* cola;
    Semaforo semaforoCaja;
    Semaforo semaforoSurtidores;
    Semaforo semaforoCola;

public:

    Empleado() {
        unsigned int surtidores = EstacionDeServicio::getInstancia()->getSurtidores();

        this->caja.crear((char*)MEMORIA_CAJA, 'C');
        this->surtidores.crear((char*)MEMORIA_SURTIDORES, 'S', surtidores);

        Semaforo semaforoCaja((char*)SEMAFORO_CAJA);
        Semaforo semaforoSurtidores((char*)SEMAFORO_SURTIDOR);

        this->semaforoCaja=semaforoCaja;
        this->semaforoSurtidores=semaforoSurtidores;

        Semaforo semaforoCola((char*) SEMAFORO_COLA);
        this->semaforoCola=semaforoCola;

        Cola<mensaje> *cola= new Cola<mensaje>( COLA_MENSAJES,'C');
        this->cola=cola;
    }

    void atenderAuto(Auto& a) {
        float tiempoDeCarga = a.getCapacidad();
        for (unsigned int i = 0; i < surtidores.cantidad(); i++) {
            semaforoSurtidores.p(i);
            if (this->surtidores.leer(i) == true) {
                this->surtidores.escribir(false, i);
                semaforoSurtidores.v(i);

                Logger::debug(getpid(), "Evento -> Atendiendo auto\n");
                Utils<int> utils;
                Logger::debug(getpid(),"Usando surtidor "  + utils.toString(i) +"\n");

                sleep(tiempoDeCarga);

                semaforoCaja.p(0);
                unsigned int saldo = caja.leer();
                caja.escribir(saldo + tiempoDeCarga);
                semaforoCaja.v(0);

                Logger::debug(getpid(),"Saldo de caja: "  + utils.toString(saldo + tiempoDeCarga) +"\n");
                string tipo=((a.getTipo()==AUTO_VIP)?"VIP":" ");
                Logger::debug(getpid(), "Auto " + tipo + " atendido\n");
                semaforoSurtidores.p(i);
                this->surtidores.escribir(true, i);
                semaforoSurtidores.v(i);
                a.setAtendido(true);
                return;
            }
            semaforoSurtidores.v(i);
        }
    }

    ~Empleado() {
        this->caja.liberar();
        this->surtidores.liberar();
        delete(this->cola);
    }

};

#endif // EMPLEADO_H
