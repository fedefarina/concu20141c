#ifndef CAJA_H
#define CAJA_H

#include "MemoriaCompartida.h"
#include "Semaforo.h"
#include "Constantes.h"

class Caja {

private:
    MemoriaCompartida<short int> caja;
    MemoriaCompartida<bool> ocupada;
    Semaforo semaforoCaja;
    Semaforo semaforoCajaOcupada;

public:
    Caja() {
        Semaforo semaforoCaja((char*)SEMAFORO_CAJA);
        this->semaforoCaja=semaforoCaja;

        Semaforo semaforoCajaOcupada((char*)SEMAFORO_CAJA_OCUPADA);
        this->semaforoCajaOcupada=semaforoCajaOcupada;

        this->caja.crear((char*)MEMORIA_CAJA, 'C');
        this->ocupada.crear((char*)MEMORIA_CAJA_OCUPADA, 'C');

    }

    bool isOcupada(){
        semaforoCajaOcupada.p();
        bool estaOcupada=ocupada.leer();
        semaforoCajaOcupada.v();
        return estaOcupada;
    }

    void setOcupada(bool isOcupada){
        semaforoCajaOcupada.p();
        ocupada.escribir(isOcupada);
        semaforoCajaOcupada.v();
    }

    unsigned int getSaldo() {
        semaforoCaja.p();
        unsigned int saldo = caja.leer();
        semaforoCaja.v();
        return saldo;
    }

    unsigned int consultarSaldo() {
        this->setOcupada(true);
        unsigned int saldo=this->getSaldo();
        sleep(1);
        this->setOcupada(false);
        return saldo;
    }

    void setMonto(unsigned int monto){
        semaforoCaja.p();
        unsigned int saldo = caja.leer();
        caja.escribir(monto + saldo);
        semaforoCaja.v();
    }

    void depositarMonto(unsigned int monto) {
        this->setOcupada(true);
        this->setMonto(monto);
        sleep(1);
        this->setOcupada(false);
    }

    ~Caja() {
        this->caja.liberar();
        this->ocupada.liberar();
    }
};

#endif // CAJA_H
