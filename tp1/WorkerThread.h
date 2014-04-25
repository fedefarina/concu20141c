#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H
#include <queue>
#include "QThread"
#include "QPushButton"
#include "QTextEdit"
#include "QTime"
#include "QLCDNumber"
#include "mainwindow.h"
#include "EstacionDeServicio.h"
#include "JefeDeEstacion.h"
#include "Logger.h"
#include "Auto.h"

class WorkerThread : public QThread{
    Q_OBJECT

    void run() Q_DECL_OVERRIDE {

        MainWindow* mainWindow=MainWindow::getInstance();
        QLCDNumber* displayNumber=mainWindow->findChild<QLCDNumber*>("timeDisplay");

        int surtidores=mainWindow->getNumeroSurtidores();
        int empleados=mainWindow->getNumeroEmpleados();
        int tiempoSimulacion=mainWindow->getTiempoSimulacion();
        EstacionDeServicio::getInstancia()->setSurtidores(surtidores);
        queue<Auto*> autos;
        JefeDeEstacion jefe;
        jefe.setEmpleados(empleados);

        Logger::debug(getpid(), "Inicio de simulacion\n");
        QTime timeElapsed;
        timeElapsed.start();

        while (tiempoSimulacion > timeElapsed.elapsed()/1000) {

            if(timeElapsed.elapsed()%1000==0)
                displayNumber->display(tiempoSimulacion - timeElapsed.elapsed()/1000);

            if (timeElapsed.elapsed()%2000==0) {
                autos.push(new Auto());
            }

            if (autos.size() > 0) {
                Auto* a = autos.front();
                jefe.recibirAuto(*a);
                autos.pop();
            }
        }

        displayNumber->display(0);
        Logger::debug(getpid(), string("Fin de simulacion\n"));
        mainWindow->writeToStdOuT("Fin de la simulación");
        EstacionDeServicio::destruirInstancia();
        emit finishSignal();
    }

signals:
    void finishSignal();
public slots:
    void onFinished(){
        MainWindow* mainWindow=MainWindow::getInstance();
        QPushButton *ejecutarButton = mainWindow->findChild<QPushButton*>("iniciarButton");
        ejecutarButton->setEnabled(true);
    }
};
#endif // WORKERTHREAD_H
