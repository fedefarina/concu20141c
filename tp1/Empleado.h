#ifndef EMPLEADO_H
#define EMPLEADO_H

#include "EstacionDeServicio.h"
#include "Auto.h"
#include "MemoriaCompartida.h"
#include "Logger.h"
#include "Semaforo.h"
#include "Caja.h"
#include "mainwindow.h"
#include "SigUnusedHandler.h"
#include "SignalHandler.h"
#include "MensajeCaja.h"

class Empleado{

private:    

    unsigned int id;
    Caja* caja;
    Cola<mensajeCaja>* colaCaja;
    Semaforo semaforoSurtidores;
    MemoriaCompartida<bool> surtidores;

public:

    Empleado(unsigned int empleadoID) {
        unsigned int surtidores = EstacionDeServicio::getInstancia()->getSurtidores();
        id=empleadoID;

        this->surtidores.crear((char*)MEMORIA_SURTIDORES, 'S', surtidores);

        Semaforo semaforoSurtidores((char*)SEMAFORO_SURTIDOR);
        this->semaforoSurtidores=semaforoSurtidores;

        this->caja = new Caja();

        Cola<mensajeCaja> *colaCaja =new Cola<mensajeCaja>( COLA_CAJA,'C');
        this->colaCaja=colaCaja;

    }

    int getSurtidorLibre() {
        int id = -1;
        for (unsigned int i = 0; i < surtidores.cantidad(); i++) {
            semaforoSurtidores.p(i);
            if(this->surtidores.leer(i) == true) {
                id = i;
                semaforoSurtidores.v(i);
                break;
            }
            semaforoSurtidores.v(i);
        }
        return id;
    }

    void atenderAuto(Auto& unAuto) {
        float tiempoDeCarga = unAuto.getCapacidad();
        for (unsigned int i = 0; i < surtidores.cantidad(); i++) {
            semaforoSurtidores.p(i);
            if (this->surtidores.leer(i) == true) {
                this->surtidores.escribir(false, i);
                semaforoSurtidores.v(i);

                string tipo=(unAuto.getTipo()==AUTO_VIP?" VIP":"");
                Utils<int> utils;
                Logger::debug(getpid(), "Evento -> Empleado "+utils.toString(id)+" atendiendo auto"+tipo+"\n");
                Logger::debug(getpid(),"Usando surtidor "  + utils.toString(i+1) +"\n");
                sleep(tiempoDeCarga);

                mensajeCaja msg;
                msg.mtype=EMPLEADO;
                msg.empleadoPID=getpid();

                colaCaja->escribir(msg);

                Logger::debug(getpid(),"El empleado "+ utils.toString(id)+ " pide usar la caja\n");
                colaCaja->leer((int) getpid(),&msg);

                Logger::debug(getpid(),"El empleado "+ utils.toString(id) + " usa la caja\n");
                caja->depositarMonto(tiempoDeCarga);
                sleep(1);
                Logger::debug(getpid(),"Saldo de caja: "  + utils.toString(caja->getSaldo()) +"\n");
                Logger::debug(getpid(),"El empleado "+ utils.toString(id) +" termino de usar la caja\n");

                semaforoSurtidores.p(i);
                this->surtidores.escribir(true, i);
                semaforoSurtidores.v(i);

                Logger::debug(getpid(), "Auto" + tipo + " atendido\n");
                return;
            }
            semaforoSurtidores.v(i);
        }
    }

    ~Empleado() {
        this->surtidores.liberar();
        delete(this->colaCaja);
        delete(this->caja);
    }

};

#endif // EMPLEADO_H
