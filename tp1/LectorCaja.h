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
            return true;
        }

        if(msg.mtype==FIN_SIMULACION){
            return false;
        }

        if(msg.mtype==ADMINISTRADOR){
            Logger::debug(getpid(),"Lector caja, atendiendo peticion del administrador\n");
            Logger::debug(getpid(),"El administrador usa la caja\n");
            administrador->consultarCaja();
            Logger::debug(getpid(),"El administrador termino de usar la caja\n");
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
