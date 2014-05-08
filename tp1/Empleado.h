#ifndef EMPLEADO_H
#define EMPLEADO_H

#include "Auto.h"
#include "Constantes.h"
#include "MemoriaCompartida.h"
#include "Logger.h"
#include "Semaforo.h"


class Empleado {


private:
    Semaforo semaforoCaja;
    Semaforo semaforoSurtidor;
    MemoriaCompartida<unsigned int> caja;

public:

    Empleado() {
        this->caja.crear((char*)"/bin/ls", 'C');        
        Semaforo semaforoCaja((char*)SEMAFORO_CAJA);
        Semaforo semaforoSurtidor((char*)SEMAFORO_SURTIDOR);
        this->semaforoCaja=semaforoCaja;
        this->semaforoSurtidor=semaforoSurtidor;
    }

    void atenderAuto(Auto a) {
        Logger::debug(getpid(), "Evento -> Atendiendo auto\n");
        float tiempoDeCarga = a.getCapacidad();

        cout<<"<<-Surtidor Value: "<<semaforoSurtidor.getValue()<<endl;
        semaforoSurtidor.p();
        cout<<"Surtidor Value: "<<semaforoSurtidor.getValue()<<"->>"<<endl;
        sleep(tiempoDeCarga);
        semaforoSurtidor.v();

        semaforoCaja.p();
        unsigned int saldo=caja.leer();
        caja.escribir(saldo+tiempoDeCarga);
        cout << "Caja: " << saldo << endl;
        semaforoCaja.v();

        Logger::debug(getpid(), "Auto atendido\n");
    }

    ~Empleado() {
        this->caja.liberar();
    }

};

#endif // EMPLEADO_H

