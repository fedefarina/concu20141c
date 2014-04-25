#ifndef EMPLEADO_H
#define EMPLEADO_H

#include "Auto.h"
#include "MemoriaCompartida.h"
#include "EstacionDeServicio.h"

enum Estado {Libre, Ocupado};

class Empleado {

private:
    Estado estado;
    MemoriaCompartida<float> caja;
    MemoriaCompartida<bool> surtidores;

public:
    Empleado() {
        this->estado = Libre;
        this->caja.crear((char*)"/bin/ls", 33);

        this->surtidores.crear((char*)"/bin/ls", 34, EstacionDeServicio::getInstancia()->getSurtidores());
    }

    void atenderAuto(Auto a) {
        this->estado = Ocupado;
        unsigned int surtidores = EstacionDeServicio::getInstancia()->getSurtidores();

        while (this->estado == Ocupado) {

            for(unsigned int i = 0; i < surtidores; i++) {

                if (this->surtidores.leer(i) == true) {
                    float tiempoDeCarga = a.getCapacidad();

                    sleep(tiempoDeCarga);

                    float recaudacion = this->caja.leer();
                    this->caja.escribir(recaudacion + tiempoDeCarga);

                    this->estado = Libre;

                    break;
                }
            }
        }
    }

    Estado getEstado() {
        return this->estado;
    }

    ~Empleado() {
        //this->caja.liberar();
    }

};

#endif // EMPLEADO_H
