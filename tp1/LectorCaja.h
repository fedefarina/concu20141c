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
    Cola<mensajeCaja> *colaCaja;

public:

    LectorCaja() {

        administrador=new Administrador();
        Cola<mensajeCaja> *colaCaja =new Cola<mensajeCaja>( COLA_CAJA,'C');
        this->colaCaja=colaCaja;
        this->caja = new Caja();
    }

    bool recibirPeticionesCaja(){


        mensajeCaja msg;
        if(colaCaja->leer(&msg)==-1){
            Logger::debug(getpid(),"Leo mal\n");
            return true;
        }

        Logger::debug(getpid(),"Lei un mensaje \n");

        if(msg.mtype==FIN_SIMULACION){
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

        return true;
    }

    ~LectorCaja() {
        delete(this->colaCaja);
        delete(administrador);
        delete(caja);
    }
};

#endif // LECTORCAJA_H
