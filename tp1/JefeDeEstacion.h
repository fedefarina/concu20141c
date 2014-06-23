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

    MemoriaCompartida<unsigned int> contadorVIP;
    Semaforo semaforoContadorVIP;

    Semaforo semaforoEmpleados;
    Semaforo semaforoColaAutos;

    Cola<mensaje> *colaAutos;

    bool leerColaAutos(unsigned int tipo,mensaje &msg){
        semaforoColaAutos.p();
        ssize_t bytesLeidos =colaAutos->leer(tipo,&msg);
        semaforoColaAutos.v();
        return bytesLeidos>0;
    }

    bool leerAuto(Auto &unAuto){

        bool autoLeido=false;
        mensaje msg;

        if(!leerColaAutos(AUTO_VIP,msg)){
            if(leerColaAutos(AUTO,msg)){
                unAuto.setTipo(AUTO);
                autoLeido=true;
            }
        }else{
            semaforoContadorVIP.p();
            unsigned int contador=contadorVIP.leer();
            contadorVIP.escribir(++contador);
            semaforoContadorVIP.v();
            autoLeido=true;
            unAuto.setTipo(AUTO_VIP);
        }

        if(autoLeido)
            unAuto.setCapacidad(msg.capacidad);

        return autoLeido;
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

public:

    JefeDeEstacion() {
        unsigned int empleados = EstacionDeServicio::getInstancia()->getEmpleados();

        this->empleados.crear((char*) MEMORIA_EMPLEADOS, 'E',empleados);

        Semaforo semaforoEmpleados((char*)SEMAFORO_EMPLEADOS);
        this->semaforoEmpleados = semaforoEmpleados;

        Semaforo semaforoColaAutos((char*) SEMAFORO_COLA_AUTOS);
        this->semaforoColaAutos=semaforoColaAutos;

        Semaforo semaforoContadorVIP((char*) SEMAFORO_CONTADOR_VIP);
        this->semaforoContadorVIP=semaforoContadorVIP;

        this->contadorVIP.crear((char*) MEMORIA_CONTADOR_VIP, 'M');

        Cola<mensaje> *colaAutos =new Cola<mensaje>( COLA_AUTOS,'C');
        this->colaAutos=colaAutos;
    }

    void recibirAuto() {

        Auto unAuto;
        unAuto.setCapacidad(0);
        unAuto.setTipo(AUTO);

        if(leerAuto(unAuto)){
            int idEmpleado = getEmpleadoLibre();

            //Busco un empleado libre
            if(idEmpleado >= 0){
                unsigned int id=idEmpleado;

                pid_t pid = fork();
                if (pid == 0) {
                    Utils<int> utils;
                    string tipo=(unAuto.getTipo()==AUTO_VIP?" VIP":"");
                    Logger::debug(getpid(),"El auto"+ tipo +" es atendido por el empleado " + utils.toString(id+1)+"\n");

                    Empleado* empleado = new Empleado(id+1);

                    while(!unAuto.isAtendido()){
                        semaforoContadorVIP.p();
                        unsigned int contador=contadorVIP.leer();
                        semaforoContadorVIP.v();

                        if(unAuto.getTipo()==AUTO_VIP || contador==0){
                            empleado->atenderAuto(unAuto);
                        }
                    }

                    if(unAuto.getTipo()==AUTO_VIP){
                        semaforoContadorVIP.p();
                        unsigned int contador=contadorVIP.leer();
                        contadorVIP.escribir(--contador);
                        semaforoContadorVIP.v();
                    }

                    delete(empleado);
                    semaforoEmpleados.p(id);
                    this->empleados.escribir(true,id);
                    semaforoEmpleados.v(id);
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

    ~JefeDeEstacion() {
        this->empleados.liberar();
        this->contadorVIP.liberar();
        delete(this->colaAutos);
    }

};

#endif // JEFEDEESTACION_H
