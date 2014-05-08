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

    JefeDeEstacion(unsigned int empleados){
        this->empleados.crear("/bin/ls", 'E', empleados);
        for(unsigned int i = 0; i < this->empleados.cantidad(); i++)
            this->empleados.escribir(true,i);
    }

    void recibirAuto(Auto unAuto) {

        Logger::debug(getpid(), "Evento > Un nuevo auto entra a la estacion de servicio\n");
        int pos = -1;

        //Busco un empleado libre
        for(unsigned int i = 0; i < this->empleados.cantidad(); i++) {
            if (this->empleados.leer(i) == true) {
                pos = i;
                break;
            }
        }
        //Si hay empleados libres.
        if (pos >= 0) {
            pid_t pid = fork();
            if (pid == 0) {
                this->empleados.escribir(false, pos);
                Empleado empleado;
                empleado.atenderAuto(unAuto);
                this->empleados.escribir(true, pos);
                exit(0);
            }
        } else {
            Logger::debug(getpid(), "Evento > No hay empleados disponibles\n");
        }
    }

    ~JefeDeEstacion() {
        this->empleados.liberar();
    }

};

#endif // JEFEDEESTACION_H

