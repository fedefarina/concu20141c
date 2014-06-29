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

    void recibirPeticionesCaja(){

        semaforoCajaOcupada.p();

        mensajeCaja msg;
        if(colaCaja->leer(&msg)==-1){
            semaforoCajaOcupada.v();
            return;
        }

        int empleadoPID=msg.empleadoPID;

        pid_t pid = fork ();
        if ( pid == 0 ) {
            if(msg.mtype==ADMINISTRADOR){
                Logger::debug(getpid(),"El administrador usa la caja\n");
                administrador->consultarCaja();
                sleep(1);
                Logger::debug(getpid(),"El administrador termino de usar la caja\n");
            }else{

            }
            semaforoCajaOcupada.v();
            exit(0);
        }
    }

    ~LectorCaja() {
        delete(this->colaCaja);
        delete(administrador);
        delete(caja);
    }
};

#endif // LECTORCAJA_H
