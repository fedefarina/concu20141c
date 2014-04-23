#ifndef JEFEDEESTACION_H
#define JEFEDEESTACION_H

#include <vector>
#include "Auto.h"
#include "Empleado.h"
#include "MemoriaCompartida.h"
using namespace std;

class JefeDeEstacion {

private:
    vector<Empleado*> empleados;
    MemoriaCompartida<float> caja;

public:
    JefeDeEstacion() {
        this->caja.crear("/bin/ls", 33);
        this->caja.escribir(0);
    }

    void recibirAuto(Auto a) {
        Empleado* empleado = NULL;
        for(unsigned int i = 0; i < empleados.size(); i++) {
            Empleado* empl = empleados.at(i);
            if (empl->getEstado() == Libre) {
                empleado = empl;
                break;
            }
        }

        if (empleado != NULL) {
            pid_t pid = fork();
            if (pid == 0) {
                sleep(1);
                empleado->atenderAuto(a);
            }
            else {
                wait();
            }
        }
    }

    void setEmpleados(unsigned int empleados) {
        for (unsigned int i = 0; i < empleados; i++) {
            this->empleados.push_back(new Empleado());
        }
    }

    ~JefeDeEstacion() {
        this->caja.liberar();
    }

};

#endif // JEFEDEESTACION_H
