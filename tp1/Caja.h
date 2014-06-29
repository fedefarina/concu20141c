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
    }

    unsigned int getSaldo() {
        semaforoCaja.p();
        unsigned int saldo = caja.leer();
        semaforoCaja.v();
        return saldo;
    }

    void setMonto(unsigned int monto){
        semaforoCaja.p();
        unsigned int saldo = caja.leer();
        caja.escribir(monto + saldo);
        semaforoCaja.v();
    }


    unsigned int consultarSaldo() {
        return this->getSaldo();
    }

    void depositarMonto(unsigned int monto) {
        this->setMonto(monto);
    }

    ~Caja() {
        this->caja.liberar();
        this->ocupada.liberar();
    }
};

#endif // CAJA_H
