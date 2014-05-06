#ifndef JEFEDEESTACION_H
#define JEFEDEESTACION_H

#include <vector>
#include "Auto.h"
#include "Empleado.h"
#include "MemoriaCompartida.h"
#include "Logger.h"
#include "mainwindow.h"
#include "FifoLectura.h"

using namespace std;


class JefeDeEstacion {

private:
    MemoriaCompartida<bool> empleados;

public:

    JefeDeEstacion(){
    }

    void recibirAuto(Auto unAuto) {

        Logger::debug(getpid(), "Evento > Un nuevo auto entra a la estacion de servicio\n");
        Empleado empleado;
        empleado.atenderAuto(unAuto);
        //        int pos = -1;

        //        //Busco un empleado libre
        //        for(unsigned int i = 0; i < this->empleados.cantidad(); i++) {
        //            if (this->empleados.leer(i) == true) {
        //                pos = i;
        //                break;
        //            }
        //        }
        //        //Si hay empleados libres.
        //        if (pos >= 0) {
        //            pid_t pid = fork();
        //            if (pid == 0) {
        //                this->empleados.escribir(false, pos);

        //                Empleado* empleado = new Empleado();
        //                empleado->atenderAuto(unAuto);

        //                this->empleados.escribir(true, pos);
        //                delete(empleado);

        //                exit(0);
        //            }
        //        } else {
        //            Logger::debug(getpid(), "Evento > No hay empleados disponibles\n");
        //        }
    }

    void setEmpleados(unsigned int empleados) {
        this->empleados.crear("/bin/ls", 40, empleados);

        for (unsigned int i = 0; i < empleados; i++)
            this->empleados.escribir(true, i);

    }

    ~JefeDeEstacion() {
        this->empleados.liberar();
    }

};

#endif // JEFEDEESTACION_H

