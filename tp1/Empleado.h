#ifndef EMPLEADO_H
#define EMPLEADO_H

#include "EstacionDeServicio.h"
#include "Auto.h"
#include "Constantes.h"
#include "MemoriaCompartida.h"
#include "Logger.h"
#include "Semaforo.h"

class Empleado {

private:    
    MemoriaCompartida<short int> caja;
    MemoriaCompartida<bool> surtidores;

    Semaforo semaforoCaja;
    Semaforo semaforoSurtidores;

public:

    Empleado() {        
        unsigned int surtidores = EstacionDeServicio::getInstancia()->getSurtidores();

        this->caja.crear((char*)MEMORIA_CAJA, 'C');
        this->surtidores.crear((char*)MEMORIA_SURTIDORES, 'S', surtidores);

        Semaforo semaforoCaja((char*)SEMAFORO_CAJA);
        Semaforo semaforoSurtidores((char*)SEMAFORO_SURTIDOR);

        this->semaforoCaja=semaforoCaja;
        this->semaforoSurtidores=semaforoSurtidores;
    }

    void atenderAuto(Auto a) {        
        float tiempoDeCarga = a.getCapacidad();
        while (true){
            for (unsigned int i = 0; i < surtidores.cantidad(); i++) {
                semaforoSurtidores.p(i);
                if (this->surtidores.leer(i) == true) {
                    this->surtidores.escribir(false, i);
                    semaforoSurtidores.v(i);

                    Logger::debug(getpid(), "Evento -> Atendiendo auto\n");

                    Utils<int> utils;
                    Logger::debug(getpid(),"Usando surtidor "  + utils.toString(i) +"\n");

                    sleep(tiempoDeCarga);

                    semaforoCaja.p(0);
                    unsigned int saldo = caja.leer();
                    caja.escribir(saldo + tiempoDeCarga);
                    semaforoCaja.v(0);

                    Logger::debug(getpid(),"Saldo de caja: "  + utils.toString(saldo + tiempoDeCarga) +"\n");
                    Logger::debug(getpid(), "Auto atendido\n");

                    semaforoSurtidores.p(i);
                    this->surtidores.escribir(true, i);
                    semaforoSurtidores.v(i);
                    return;
                }
                semaforoSurtidores.v(i);
            }
        }
    }

    ~Empleado() {
        this->caja.liberar();
        this->surtidores.liberar();
    }

};

#endif // EMPLEADO_H
