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

        semaforoSurtidoresDisponibles.p();

        mensaje msg;
        if(colaAutos->leer(&msg)==-1){
            semaforoSurtidoresDisponibles.v();
            return true;
        }

        if(msg.mtype==FIN_SIMULACION){
            semaforoSurtidoresDisponibles.v();
            return false;
        }

        Auto unAuto;

        unAuto.setTipo(msg.mtype);
        unAuto.setCapacidad(msg.capacidad);

        unsigned int id=msg.empleadoID;


        pid_t pid = fork();
        if (pid == 0) {

            Utils<int> utils;
            string tipo=(unAuto.getTipo()==AUTO_VIP?" VIP":"");
            Logger::debug(getpid(),"El auto"+ tipo +" es atendido por el empleado " + utils.toString(id+1)+"\n");

            Empleado* empleado = new Empleado(id+1);
            empleado->atenderAuto(unAuto);
            semaforoSurtidoresDisponibles.v();

            delete(empleado);
            semaforoEmpleados.p(id);
            this->empleados.escribir(true,id);
            semaforoEmpleados.v(id);
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
