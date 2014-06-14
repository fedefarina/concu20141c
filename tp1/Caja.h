#ifndef CAJA_H
#define CAJA_H

#include "MemoriaCompartida.h"
#include "Constantes.h"

class Caja {

private:
    MemoriaCompartida<short int> caja;
    Semaforo semaforoCaja;

public:
    Caja() {        
        Semaforo semaforoCaja((char*)SEMAFORO_CAJA);
        this->semaforoCaja=semaforoCaja;
        this->caja.crear((char*)MEMORIA_CAJA, 'C');
    }

    unsigned int getSaldo() {
        semaforoCaja.p();
        unsigned int saldo = caja.leer();
        semaforoCaja.v();
        return saldo;
    }

    void depositarMonto(unsigned int monto) {
        semaforoCaja.p();
        unsigned int saldo = caja.leer();
        caja.escribir(monto + saldo);
        semaforoCaja.v();
    }

    ~Caja() {
        this->caja.liberar();
    }
};

#endif // CAJA_H
