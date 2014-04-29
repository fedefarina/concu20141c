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
    vector<Empleado*> empleados;
    MemoriaCompartida<float> caja;

public:

    JefeDeEstacion(){
        this->caja.crear((char*)"/bin/ls", 33);
        this->caja.escribir(0);
    }

    void recibirAuto(Auto unAuto) {

        Logger::debug(getpid(), "Evento > Un nuevo auto entra a la estacion de servicio\n");
        int pos = -1;

        //Busco un empleado libre
        for(unsigned int i = 0; i < empleados.size(); i++) {
            if (((Empleado*)empleados.at(i))->getEstado() == Libre) {
                pos = i;
                break;
            }
        }
        //Si hay empleados libres.
        if (pos >= 0) {
            pid_t pid = fork();
            if (pid == 0) {
                ((Empleado*) empleados.at(pos))->atenderAuto(unAuto);
                exit(0);
            }else{
                wait();
            }
        } else {
            Logger::debug(getpid(), "Evento > No hay empleados disponibles\n");
        }
    }

    void setEmpleados(unsigned int empleados) {
        for (unsigned int i = 0; i < empleados; i++) {
            this->empleados.push_back(new Empleado());
        }
    }

    ~JefeDeEstacion() {
        this->caja.liberar();
    }

};

#endif // JEFEDEESTACION_H
