#ifndef JEFEDEESTACION_H
#define JEFEDEESTACION_H

#include "Auto.h"
#include "Empleado.h"
#include "Cola.h"
#include "Mensajes.h"
#include <sys/wait.h>

using namespace std;

class JefeDeEstacion {

private:
    MemoriaCompartida<bool> empleados;
    Semaforo semaforoEmpleados;
    Semaforo semaforoSurtidoresDisponibles;

    Cola<mensaje> *colaAutos;


public:

    JefeDeEstacion() {
        unsigned int empleados = EstacionDeServicio::getInstancia()->getEmpleados();

        this->empleados.crear((char*) MEMORIA_EMPLEADOS, 'E',empleados);

        Semaforo semaforoEmpleados((char*)SEMAFORO_EMPLEADOS);
        this->semaforoEmpleados = semaforoEmpleados;

        Semaforo semaforoSurtidoresDisponibles((char*)SEMAFORO_SURTIDORES_DISPONIBLES);
        this->semaforoSurtidoresDisponibles=semaforoSurtidoresDisponibles;
        Utils<int> utils;

        Cola<mensaje> *colaAutos =new Cola<mensaje>( COLA_AUTOS,'C');
        this->colaAutos=colaAutos;
    }

    bool recibirAuto() {

        Auto unAuto;
        mensaje msg;

        Utils<int> utils;
        //Logger::debug(getpid(),"Afuera Valor Antes : "+utils.toString(semaforoSurtidoresDisponibles.getValue())+"\n");
        semaforoSurtidoresDisponibles.p();

        Logger::debug(getpid(),"Antes de leer\n");
        if(colaAutos->leer(&msg)==-1){
            semaforoSurtidoresDisponibles.v();
            return true;
        }

        Logger::debug(getpid(),"Capacidad: " +utils.toString(msg.capacidad)+ "\n");


        if(msg.mtype==FIN_SIMULACION){
            semaforoSurtidoresDisponibles.v();
            return false;
        }


        unAuto.setTipo(msg.mtype);
        unAuto.setCapacidad(msg.capacidad);

        unsigned int id=msg.empleadoID;

        //unsigned int id=getEmpleadoLibre();

        pid_t pid = fork();
        if (pid == 0) {

            //                    string tipo=(unAuto.getTipo()==AUTO_VIP?" VIP":"");

            //                    Utils<int> utils;
            //                    Logger::debug(getpid(),"El auto"+ tipo +" es atendido por el empleado " + utils.toString(id+1)+"\n");


            Empleado* empleado = new Empleado(id+1);
            empleado->atenderAuto(unAuto);
            delete(empleado);
            semaforoEmpleados.p(id);
            this->empleados.escribir(true,id);
            semaforoEmpleados.v(id);

//            sleep(3);
            semaforoSurtidoresDisponibles.v();
            exit(0);
        }

        return true;
    }

    void setEmpleadoOcupado(unsigned int id){
        semaforoEmpleados.p(id);
        this->empleados.escribir(false,id);
        semaforoEmpleados.v(id);
    }

    int getEmpleadoLibre() {
        int id = -1;
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
        return id;
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
        this->empleados.liberar();
        delete(this->colaAutos);
    }

};

#endif // JEFEDEESTACION_H
