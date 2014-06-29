#ifndef LECTORCAJA_H
#define LECTORCAJA_H

#include "Constantes.h"
#include "Semaforo.h"
#include "Caja.h"
#include "Cola.h"
#include "Administrador.h"
#include "Mensajes.h"

class LectorCaja {

private:

    Caja* caja;

    Administrador *administrador;
    Semaforo semaforoColaCaja;
    Cola<mensaje> *colaCaja;

    MemoriaCompartida<bool> cajaFinalizada;
    Semaforo semaforoCajaFinalizada;

    bool leerColaCaja(mensaje &msg){
//        semaforoColaCaja.p();
//        ssize_t bytesLeidos =colaCaja->leer(&msg);
//        semaforoColaCaja.v();
//        return bytesLeidos>0;
        return true;
    }

    void finalizarCaja(){
//        if(!hayPeticionesPendientes() && !isCajaFinalizada()){
//            semaforoCajaFinalizada.p();
//            cajaFinalizada.escribir(true);
//            semaforoCajaFinalizada.v();
//        }
    }

public:

    LectorCaja() {

        administrador=new Administrador();
        this->cajaFinalizada.crear((char*) MEMORIA_CAJA_FINALIZADA, 'M');

        Semaforo semaforoCajaFinalizada((char*) SEMAFORO_CAJA_FINALIZADA);
        this->semaforoCajaFinalizada=semaforoCajaFinalizada;

        Semaforo semaforoColaCaja((char*) SEMAFORO_COLA_CAJA);
        this->semaforoColaCaja=semaforoColaCaja;

        Cola<mensaje> *colaCaja =new Cola<mensaje>( COLA_CAJA,'C');
        this->colaCaja=colaCaja;

        this->caja = new Caja();
    }

    void recibirPeticionesCaja(){

//        if(caja->isOcupada())
//            return;

//        if(finalizado)
//            finalizarCaja();

//        mensaje msg;

//        if(leerColaCaja(ADMINISTRADOR,msg)){
//            pid_t pid = fork ();
//            if ( pid == 0 ) {
//                administrador->consultarCaja();
//                exit(0);
//            }
//        }else{
//            if(leerColaCaja(EMPLEADO,msg)){
//                pid_t pid = fork ();
//                if ( pid == 0 ) {
//                    pid_t pidEmpleado=msg.pid;
//                    kill(pidEmpleado,SIGALRM);
//                    exit(0);
//                }
//            }
//        }
    }

    bool isCajaFinalizada(){
        semaforoCajaFinalizada.p();
        bool finalizada=cajaFinalizada.leer();
        semaforoCajaFinalizada.v();
        return finalizada;
    }

    ~LectorCaja() {
        this->cajaFinalizada.liberar();
        delete(this->colaCaja);
        delete(administrador);
        delete(caja);
    }
};

#endif // LECTORCAJA_H
