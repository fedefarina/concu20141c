#ifndef JEFEDEESTACION_H
#define JEFEDEESTACION_H

#include <vector>
#include "Auto.h"
#include "Empleado.h"
#include "MemoriaCompartida.h"
#include "Logger.h"
#include "mainwindow.h"
#include "FifoLectura.h"
#include <sys/wait.h>
using namespace std;

class JefeDeEstacion {

private:
    MemoriaCompartida<bool> empleados;
    Semaforo semaforoEmpleados;

public:

    JefeDeEstacion() {
        unsigned int empleados = EstacionDeServicio::getInstancia()->getEmpleados();
        this->empleados.crear((char*) MEMORIA_EMPLEADOS, 'E', empleados);
        Semaforo semaforoEmpleados((char*)SEMAFORO_EMPLEADOS,1,empleados);
        this->semaforoEmpleados = semaforoEmpleados;

        for (unsigned int i = 0; i < empleados; i++)
            this->empleados.escribir(true, i);

    }

    void recibirAuto(Auto unAuto) {
        int id = -1;
        unsigned int empleados = EstacionDeServicio::getInstancia()->getEmpleados();
        Logger::debug(getpid(), "Evento > Un nuevo auto entra a la estacion de servicio\n");

        //Busco un empleado libre
        for (unsigned int i = 0; i < empleados; i++) {
            semaforoEmpleados.p(i);
            if(this->empleados.leer(i) == true) {
                id = i;
                semaforoEmpleados.v(i);
                break;
            }
            semaforoEmpleados.v(i);
        }

        //Si hay empleados libres.
        if (id >= 0) {
            pid_t pid = fork();
            if (pid == 0) {
                cout << "El auto es atendido por el empleado " << id << endl;
                semaforoEmpleados.p(id);
                this->empleados.escribir(false, id);
                semaforoEmpleados.v(id);

                Empleado* empleado = new Empleado();
                empleado->atenderAuto(unAuto);
                delete(empleado);

                semaforoEmpleados.p(id);
                this->empleados.escribir(true,id);
                semaforoEmpleados.v(id);
                exit(0);
            }
        } else {
            Logger::debug(getpid(), "Evento > No hay empleados disponibles\n");
            Logger::debug(getpid(), "Evento > El auto se retira de la estación de servicio\n");
        }
    }

    unsigned int getEmpleadosOcupados() {
        unsigned int ocupados = 0;
        for (unsigned int i = 0; i < empleados.cantidad(); i++) {
            semaforoEmpleados.p(i);
            if (this->empleados.leer(i) == false) {
                ocupados++;
                semaforoEmpleados.v(i);
            }
            else {
                semaforoEmpleados.v(i);
            }
        }
        return ocupados;
    }

    ~JefeDeEstacion() {
        Logger::debug(getpid(), "Murio jefe\n");
        this->empleados.liberar();
    }

};

#endif // JEFEDEESTACION_H
