#ifndef EMPLEADO_H
#define EMPLEADO_H

#include "EstacionDeServicio.h"
#include "Auto.h"
#include "Constantes.h"
#include "MemoriaCompartida.h"
#include "Logger.h"
#include "Semaforo.h"
#include "Caja.h"
#include "SignalHandler.h"
#include "mainwindow.h"

class Empleado {

private:    
    MemoriaCompartida<bool> surtidores;
    Caja* caja;
    Cola<mensaje>* colaAutos;
    Cola<mensaje>* colaCaja;
    Semaforo semaforoSurtidores;
    Semaforo semaforoColaAutos;
    Semaforo semaforoColaCaja;
    unsigned int id;


public:

    Empleado(unsigned int id) {

        this->id=id;
        unsigned int surtidores = EstacionDeServicio::getInstancia()->getSurtidores();

        this->surtidores.crear((char*)MEMORIA_SURTIDORES, 'S', surtidores);

        Semaforo semaforoSurtidores((char*)SEMAFORO_SURTIDOR);
        this->semaforoSurtidores=semaforoSurtidores;

        Semaforo semaforoColaAutos((char*) SEMAFORO_COLA_AUTOS);
        this->semaforoColaAutos=semaforoColaAutos;

        Cola<mensaje> *colaAutos= new Cola<mensaje>( COLA_AUTOS,'C');
        this->colaAutos=colaAutos;
        this->caja = new Caja();

        Semaforo semaforoColaCaja((char*) SEMAFORO_COLA_CAJA);
        this->semaforoColaCaja=semaforoColaCaja;

        Cola<mensaje> *colaCaja =new Cola<mensaje>( COLA_CAJA,'C');
        this->colaCaja=colaCaja;
    }

    bool leerColaCaja(unsigned int tipo,mensaje &msg){
        semaforoColaCaja.p();
        ssize_t bytesLeidos =colaCaja->leer(tipo,&msg);
        semaforoColaCaja.v();
        return bytesLeidos>0;
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

                mensaje msg;
                msg.id=getpid();
                msg.capacidad=tiempoDeCarga;
                msg.mtype=EMPLEADO;
                msg.empleadoID=id;

                semaforoColaCaja.p();
                colaCaja->escribir(msg);
                semaforoColaCaja.v();

                Logger::debug(getpid(),"El empleado "+ utils.toString(id)+ " pide usar la caja\n");
                while(!leerColaCaja(SENIAL,msg));

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
        delete(this->colaCaja);
        delete(this->caja);
    }

};

#endif // EMPLEADO_H
