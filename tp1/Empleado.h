#ifndef EMPLEADO_H
#define EMPLEADO_H

#include "Auto.h"
#include "MemoriaCompartida.h"
#include "EstacionDeServicio.h"
#include "Logger.h"

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

        this->surtidores.crear((char*)"/bin/ls", 34, EstacionDeServicio::getInstance().getSurtidores());
    }

    void atenderAuto(Auto a) {
        Logger::debug(getpid(), "Evento -> Atendiendo auto\n");

        this->estado = Ocupado;

        unsigned int surtidores = EstacionDeServicio::getInstance().getSurtidores();

        while (this->estado == Ocupado) {

            for(unsigned int i = 0; i < surtidores; i++) {

                if (this->surtidores.leer(i) == true) {
                    float tiempoDeCarga = a.getCapacidad();
                    this->surtidores.escribir(false, i);

                    sleep(tiempoDeCarga);

                    float recaudacion = this->caja.leer();
                    this->caja.escribir(recaudacion + tiempoDeCarga);

                    this->surtidores.escribir(true, i);
                    this->estado = Libre;

                    Logger::debug(getpid(), "Auto atendido\n");

                    break;
                }
            }
        }
    }

    Estado getEstado() {
        return this->estado;
    }

    void setEstado(Estado estado) {
        this->estado = estado;
    }

    ~Empleado() {
        this->caja.liberar();
        this->surtidores.liberar();
    }

};

#endif // EMPLEADO_H
