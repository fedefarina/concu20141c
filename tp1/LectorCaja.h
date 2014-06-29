#ifndef LECTORCAJA_H
#define LECTORCAJA_H

#include "Constantes.h"
#include "Semaforo.h"
#include "Caja.h"
#include "Cola.h"
#include "Administrador.h"
#include "MensajeCaja.h"

class LectorCaja {

private:

    Caja* caja;

    Administrador *administrador;
    Semaforo semaforoCaja;
    Semaforo semaforoCajaOcupada;
    Cola<mensajeCaja> *colaCaja;

public:

    LectorCaja() {

        administrador=new Administrador();

        Cola<mensajeCaja> *colaCaja =new Cola<mensajeCaja>( COLA_CAJA,'C');
        this->colaCaja=colaCaja;

        Semaforo semaforoCaja((char*)SEMAFORO_CAJA);
        this->semaforoCaja=semaforoCaja;

        Semaforo semaforoCajaOcupada((char*)SEMAFORO_CAJA_OCUPADA);
        this->semaforoCajaOcupada=semaforoCajaOcupada;

        this->caja = new Caja();
    }

    bool recibirPeticionesCaja(){

        Logger::debug(getpid(),"Antes\n");
        semaforoCajaOcupada.p();
        Logger::debug(getpid(),"Despues\n");

        mensajeCaja msg;
        if(colaCaja->leer(&msg)==-1){
            Logger::debug(getpid(),"Leo mal\n");
            semaforoCajaOcupada.v();
            return true;
        }

        Logger::debug(getpid(),"Lei un mensaje \n");

        if(msg.mtype==FIN_SIMULACION){
            semaforoCajaOcupada.v();
            return false;
        }


        if(msg.mtype==ADMINISTRADOR){
            Logger::debug(getpid(),"Lector caja, atendiendo peticion del administrador\n");
            administrador->consultarCaja();
        }else{
            Logger::debug(getpid(),"Lector caja, atendiendo peticion de empleado\n");
            msg.mtype=msg.empleadoPID;
            colaCaja->escribir(msg);
        }

        sleep(1);
        semaforoCajaOcupada.v();

        return true;
    }

    ~LectorCaja() {
        delete(this->colaCaja);
        delete(administrador);
        delete(caja);
    }
};

#endif // LECTORCAJA_H
