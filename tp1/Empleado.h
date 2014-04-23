#ifndef EMPLEADO_H
#define EMPLEADO_H

#include "Auto.h"
#include "MemoriaCompartida.h"

enum Estado {Libre, Ocupado};

class Empleado {

private:
    Estado estado;
    MemoriaCompartida<float> caja;

public:
    Empleado() {
        this->estado = Libre;
        this->caja.crear("/bin/ls", 33);
    }

    void atenderAuto(Auto a) {
        this->estado = Ocupado;
        float tiempoDeCarga = a.getCapacidad();

        sleep(tiempoDeCarga);

        float recaudacion = this->caja.leer();
        this->caja.escribir(recaudacion + tiempoDeCarga);
        this->estado = Libre;
    }

    Estado getEstado() {
        return this->estado;
    }

    ~Empleado() {
        this->caja.liberar();
    }

};

#endif // EMPLEADO_H
