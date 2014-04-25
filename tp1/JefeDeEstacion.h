#ifndef JEFEDEESTACION_H
#define JEFEDEESTACION_H

#include <vector>
#include "Auto.h"
#include "Empleado.h"
#include "MemoriaCompartida.h"
#include "Logger.h"
#include "mainwindow.h"

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
        MainWindow *mainWindow=MainWindow::getInstance();
        mainWindow->writeToStdOuT(string("Evento > Un nuevo auto entra a la estacion de servicio"));
        Logger::debug(getpid(), string("Evento > Un nuevo auto entra a la estacion de servicio\n"));

        Empleado* empleado = NULL;

        //Busco un empleado libre
        for(unsigned int i = 0; i < empleados.size(); i++) {
            Empleado* empl = empleados.at(i);
            if (empl->getEstado() == Libre) {
                empleado = empl;
                break;
            }
        }

        //Si hay empleados libres.
        if (empleado != NULL) {
            mainWindow->writeToStdOuT(string("Evento > Atendiendo auto\n"));
            Logger::debug(getpid(), string("Evento > Atendiendo auto\n"));
            pid_t pid = fork();
            if (pid == 0) {
                sleep(1);
                empleado->atenderAuto(a);
            }
        }else{
            Logger::debug(getpid(), string("Evento > No hay empleados disponibles\n"));
            mainWindow->writeToStdOuT(string("Evento > No hay empleados disponibles"));
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
