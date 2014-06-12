#ifndef JEFEDEESTACION_H
#define JEFEDEESTACION_H

#include <vector>
#include "Auto.h"
#include "Empleado.h"
#include "MemoriaCompartida.h"
#include "Logger.h"
#include "mainwindow.h"
#include "Cola.h"
#include "Mensajes.h"
#include <sys/wait.h>

using namespace std;

class JefeDeEstacion {

private:
    MemoriaCompartida<bool> empleados;
    Semaforo semaforoEmpleados;
    Semaforo semaforoCola;
    Cola<mensaje> *cola;

public:

    JefeDeEstacion() {
        unsigned int empleados = EstacionDeServicio::getInstancia()->getEmpleados();
        this->empleados.crear((char*) MEMORIA_EMPLEADOS, 'E', empleados);
        Semaforo semaforoEmpleados((char*)SEMAFORO_EMPLEADOS);
        this->semaforoEmpleados = semaforoEmpleados;

        Semaforo semaforoCola((char*) SEMAFORO_COLA);
        this->semaforoCola=semaforoCola;

        Cola<mensaje> *cola =new Cola<mensaje>( COLA_MENSAJES,'C');
        this->cola=cola;
    }

    void recibirAuto() {

        //vip 0
        //comun 1

        mensaje msg;
        Auto unAuto;
        unAuto.setCapacidad(0);
        unAuto.setTipo(0);

        std::cout<<"recibir auto p value "<<semaforoCola.getValue()<<std::endl;
        semaforoCola.p();
        ssize_t bytesLeidos =cola->leer(3,&msg);
        std::cout<<"Lei cola"<<std::endl;
        semaforoCola.v2();

        std::cout<<"recibir auto p value 2 "<<semaforoCola.getValue()<<std::endl;
        if(bytesLeidos>0)
            return;

        semaforoCola.p();
        bytesLeidos =cola->leer(0,&msg);
        semaforoCola.v2();

        if(bytesLeidos<0){
            semaforoCola.p();
            bytesLeidos =cola->leer(1,&msg);
            semaforoCola.v2();

            if(bytesLeidos>0)
                unAuto.setTipo(1);
        }else{
            unAuto.setTipo(0);
        }

        if(bytesLeidos<0)
            return;

        unAuto.setCapacidad(msg.capacidad);
        Logger::debug(getpid(), "Evento > Un nuevo auto entra a la estacion de servicio\n");

        int id = -1;
        //Busco un empleado libre
        for (unsigned int i = 0; i < empleados.cantidad(); i++) {
            semaforoEmpleados.p(i);
            if(this->empleados.leer(i) == true) {
                Logger::debug(getpid(), "Evento > Guola\n");
                this->empleados.escribir(false, i);
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
                Utils<int> utils;
                Logger::debug(getpid(),"El auto es atendido por el empleado " + utils.toString(id)+"\n");

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
            }
            semaforoEmpleados.v(i);
        }
        return ocupados;
    }

    ~JefeDeEstacion() {
        Logger::debug(getpid(), "Murio jefe\n");
        this->empleados.liberar();
        delete(this->cola);
    }

};

#endif // JEFEDEESTACION_H
