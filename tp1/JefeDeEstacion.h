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


    bool leerCola(unsigned int tipo,mensaje &msg){
        semaforoCola.p();
        ssize_t bytesLeidos =cola->leer(tipo,&msg);
        semaforoCola.v();
        return bytesLeidos>0;
    }


    bool leerAuto(Auto &unAuto){

        //Vip 0
        //Comun 1
        bool autoLeido=false;
        mensaje msg;

        if(!leerCola(AUTO_VIP,msg)){
            if(leerCola(AUTO,msg)){
                Logger::debug(getpid(), "Evento > Un nuevo auto entra a la estacion de servicio\n");
                unAuto.setTipo(AUTO);
                autoLeido=true;
            }
        }else{
            Logger::debug(getpid(), "Evento > Un nuevo auto VIP entra a la estacion de servicio\n");
            autoLeido=true;
            unAuto.setTipo(AUTO_VIP);
        }

        if(autoLeido)
            unAuto.setCapacidad(msg.capacidad);

        return autoLeido;
    }

    void recibirAuto() {

        Auto unAuto;
        unAuto.setCapacidad(0);
        unAuto.setTipo(0);

        if(leerAuto(unAuto)){



            int id = -1;
            //Busco un empleado libre
            for (unsigned int i = 0; i < empleados.cantidad(); i++) {
                semaforoEmpleados.p(i);
                if(this->empleados.leer(i) == true) {
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

                    while(!unAuto.isAtendido()){
                        empleado->atenderAuto(unAuto);
                        if(!unAuto.isAtendido()){
                            mensaje msg;
                            if(leerCola(AUTO_VIP,msg)){
                                Auto autoVip;
                                autoVip.setCapacidad(msg.capacidad);
                                autoVip.setTipo(AUTO_VIP);
                                while(!autoVip.isAtendido())
                                    empleado->atenderAuto(autoVip);
                            }
                        }
                    }

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
