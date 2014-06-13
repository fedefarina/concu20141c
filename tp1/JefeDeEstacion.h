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
                Logger::debug(getpid(),"\nEvento > Un nuevo auto entra a la estacion de servicio\n");
                unAuto.setTipo(AUTO);
                autoLeido=true;
            }
        }else{
            Logger::debug(getpid(), "\nEvento > Un nuevo auto VIP entra a la estacion de servicio\n");
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
        unAuto.setTipo(AUTO);

        if(leerAuto(unAuto)){
            unsigned int id = getEmpleadoLibre();

            //Busco un empleado libre
            if(id > 0){
                pid_t pid = fork();
                if (pid == 0) {
                    Utils<int> utils;
                    string tipo=(unAuto.getTipo()==AUTO_VIP?" VIP":"");
                    Logger::debug(getpid(),"El auto"+ tipo +" es atendido por el empleado " + utils.toString(id)+"\n");

                    Empleado* empleado = new Empleado();

                    while(!unAuto.isAtendido()){
                        empleado->atenderAuto(unAuto);
                        if(unAuto.getTipo()!=AUTO_VIP && !unAuto.isAtendido()){
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
                    semaforoEmpleados.p(id-1);
                    this->empleados.escribir(true,id-1);
                    semaforoEmpleados.v(id-1);
                    exit(0);
                }
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

    unsigned int getEmpleadoLibre() {
        unsigned int id = 0;
        for (unsigned int i = 0; i < empleados.cantidad(); i++) {
            semaforoEmpleados.p(i);
            if(this->empleados.leer(i) == true) {
                this->empleados.escribir(false, i);
                id = i+1;
                semaforoEmpleados.v(i);
                break;
            }
            semaforoEmpleados.v(i);
        }

        return id;
    }

    ~JefeDeEstacion() {
        this->empleados.liberar();
        delete(this->cola);
    }

};

#endif // JEFEDEESTACION_H
