#include "JefeDeEstacion.h"
#include "mainwindow.h"
#include <QApplication>
#include <queue>
#include "Auto.h"
#include "Logger.h"

int main(int argc, char *argv[])
{
    /*QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();*/
/*
    queue<Auto*> autos;
    JefeDeEstacion jefe;
    int empleados = 4, surtidores = 3;

    jefe.setEmpleados(empleados);

    float tiempoDeSimulacion = 10;
*/

    Logger::debug(getpid(), "Inicio de simulacion\n");
/*
    while (tiempoDeSimulacion > 0) {
        tiempoDeSimulacion -= 0.1;

        srand(time(NULL));
        if (rand() % 2 == 0) {
            autos.push(new Auto());
        }

        if (autos.size() > 0) {
            Auto* a = autos.front();

            jefe.recibirAuto(*a);

            autos.pop();
        }
    }
*/
    Logger::debug(getpid(), string("Fin de simulacion\n"));

    return 0;
}
