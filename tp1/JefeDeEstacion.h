#ifndef JEFEDEESTACION_H
#define JEFEDEESTACION_H

#include <vector>
#include "Auto.h"
#include "Empleado.h"
#include "Cola.h"
#include "Mensajes.h"
#include <sys/wait.h>

using namespace std;

class JefeDeEstacion {

private:
    MemoriaCompartida<bool> empleados;
    MemoriaCompartida<bool> cajaFinalizada;
    Semaforo semaforoCajaFinalizada;

    MemoriaCompartida<unsigned int> contadorVIP;
    Semaforo semaforoContadorVIP;

    Semaforo semaforoEmpleados;
    Semaforo semaforoColaAutos;

    Cola<mensaje> *colaAutos;
    Caja* caja;

    Semaforo semaforoColaCaja;
    Cola<mensaje> *colaCaja;


    void finalizarCaja(){
        if(!hayPeticionesPendientes() && !isCajaFinalizada()){
            semaforoCajaFinalizada.p();
            cajaFinalizada.escribir(true);
            semaforoCajaFinalizada.v();
        }
    }

    bool leerColaAutos(unsigned int tipo,mensaje &msg){
        semaforoColaAutos.p();
        ssize_t bytesLeidos =colaAutos->leer(tipo,&msg);
        semaforoColaAutos.v();
        return bytesLeidos>0;
    }


    bool leerColaCaja(unsigned int tipo,mensaje &msg){
        semaforoColaCaja.p();
        ssize_t bytesLeidos =colaCaja->leer(tipo,&msg);
        semaforoColaCaja.v();
        return bytesLeidos>0;
    }

    bool leerAuto(Auto &unAuto){

        //Vip 0
        //Comun 1
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

    bool hayPeticionesPendientes(){
        semaforoColaCaja.p();
        bool estaVacia=colaCaja->estaVacia();
        semaforoColaCaja.v();
        return !estaVacia;
    }

public:

    JefeDeEstacion() {
        unsigned int empleados = EstacionDeServicio::getInstancia()->getEmpleados();

        this->empleados.crear((char*) MEMORIA_EMPLEADOS, 'E',empleados);

        this->cajaFinalizada.crear((char*) MEMORIA_CAJA_FINALIZADA, 'M');

        Semaforo semaforoEmpleados((char*)SEMAFORO_EMPLEADOS);
        this->semaforoEmpleados = semaforoEmpleados;

        Semaforo semaforoColaAutos((char*) SEMAFORO_COLA_AUTOS);
        this->semaforoColaAutos=semaforoColaAutos;

        Semaforo semaforoContadorVIP((char*) SEMAFORO_CONTADOR_VIP);
        this->semaforoContadorVIP=semaforoContadorVIP;

        Semaforo semaforoCajaFinalizada((char*) SEMAFORO_CAJA_FINALIZADA);
        this->semaforoCajaFinalizada=semaforoCajaFinalizada;

        this->contadorVIP.crear((char*) MEMORIA_CONTADOR_VIP, 'M');

        Cola<mensaje> *colaAutos =new Cola<mensaje>( COLA_AUTOS,'C');
        this->colaAutos=colaAutos;

        Semaforo semaforoColaCaja((char*) SEMAFORO_COLA_CAJA);
        this->semaforoColaCaja=semaforoColaCaja;

        Cola<mensaje> *colaCaja =new Cola<mensaje>( COLA_CAJA,'C');
        this->colaCaja=colaCaja;

        this->caja = new Caja();
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

    void recibirPeticionesCaja(bool finalizado){

        if(caja->isOcupada()){
            return;
        }

        if(finalizado)
            finalizarCaja();

        mensaje msg;

        if(leerColaCaja(ADMINISTRADOR,msg)){
            pid_t pid = fork ();
            if ( pid == 0 ) {
                Logger::debug(getpid(),"El administrador usa la caja\n");
                unsigned int saldo=caja->consultarSaldo();
                Utils<unsigned int> utils;
                Logger::debug(getpid(),"Saldo:"+ utils.toString(saldo) +"\n");
                Logger::debug(getpid(),"El administrador termino de usar la caja\n");
                exit(0);
            }
        }else{
            if(leerColaCaja(EMPLEADO,msg)){
                pid_t pid = fork ();
                if ( pid == 0 ) {
                    pid_t pidEmpleado=msg.pid;
                    unsigned int empleadoID=msg.empleadoID;
                    unsigned int monto=msg.capacidad;
                    Utils<int> utils;
                    Logger::debug(getpid(),"El empleado "+ utils.toString(empleadoID)
                                  + " usa la caja pid:"+utils.toString(pidEmpleado)+"\n");
                    caja->depositarMonto(monto);
                    //msg.mtype=SENIAL;
                    //colaCaja->escribir(msg);
                    kill(pidEmpleado,SIGALRM);
                    Logger::debug(getpid(),"El empleado "+ utils.toString(empleadoID)
                                  + " termino de usar la caja pid:"+utils.toString(pidEmpleado)+"\n");
                    exit(0);
                }
            }
        }
    }


    bool isCajaFinalizada(){
        semaforoCajaFinalizada.p();
        bool finalizada=cajaFinalizada.leer();
        semaforoCajaFinalizada.v();
        return finalizada;
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
        this->cajaFinalizada.liberar();
        delete(this->colaAutos);
        delete(this->colaCaja);
        delete(caja);
    }

};

#endif // JEFEDEESTACION_H
