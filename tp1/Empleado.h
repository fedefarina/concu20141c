#ifndef EMPLEADO_H
#define EMPLEADO_H

#include "Auto.h"
#include "MemoriaCompartida.h"
#include "EstacionDeServicio.h"
#include "Logger.h"


class Empleado {

public:

    Empleado() {
    }

    void atenderAuto(Auto a) {
        Logger::debug(getpid(), "Evento -> Atendiendo auto\n");
        //        EstacionDeServicio estacion=EstacionDeServicio::getInstance();
        //        estacion.usarSurtidor();
        //        float tiempoDeCarga = a.getCapacidad();
        //        sleep(tiempoDeCarga*10);
        //        estacion.guardarEnCaja(tiempoDeCarga);
        //        estacion.liberarSurtidor();
        Logger::debug(getpid(), "Auto atendido\n");
    }

    ~Empleado() {
    }

};

#endif // EMPLEADO_H

