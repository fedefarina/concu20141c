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
    MemoriaCompartida<unsigned int> empleados;
    MemoriaCompartida<unsigned int> caja;
    Semaforo semaforoEmpleados;
    Semaforo semaforoCaja;

public:

    JefeDeEstacion(){
        this->empleados.crear((char*) MEMORIA_EMPLEADOS, 'E');
        this->caja.crear((char*) MEMORIA_CAJA,'C');

        Semaforo semaforoCaja((char*) SEMAFORO_CAJA);
        Semaforo semaforoEmpleados((char*)SEMAFORO_EMPLEADOS,1);

        this->semaforoCaja=semaforoCaja;
        this->semaforoEmpleados=semaforoEmpleados;
    }

    void setEmpleados(unsigned int empleados) {
        this->empleados.escribir(empleados);
    }

    void recibirAuto(Auto unAuto) {

        Logger::debug(getpid(), "Evento > Un nuevo auto entra a la estacion de servicio\n");

        //Busco un empleado libre
        semaforoEmpleados.p();
        unsigned int empleadosLibres = this->empleados.leer();
        semaforoEmpleados.v();

        //Si hay empleados libres.
        if (empleadosLibres > 0) {
            pid_t pid = fork();
            if (pid == 0) {
                semaforoEmpleados.p();
                empleadosLibres=empleados.leer();
                this->empleados.escribir(empleadosLibres-1);
                semaforoEmpleados.v();

                Empleado empleado;
                empleado.atenderAuto(unAuto);

                semaforoEmpleados.p();
                empleadosLibres=empleados.leer();
                this->empleados.escribir(empleadosLibres+1);
                semaforoEmpleados.v();
                exit(0);
            }
        } else {
            Logger::debug(getpid(), "Evento > No hay empleados disponibles\n");
            Logger::debug(getpid(), "Evento > El auto se retira de la estación de servicio\n");
        }
    }

    ~JefeDeEstacion() {
        this->empleados.liberar();
        this->semaforoEmpleados.eliminar();
        this->caja.liberar();
    }

};

#endif // JEFEDEESTACION_H

