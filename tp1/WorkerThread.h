#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H
#include "QThread"
#include "QPushButton"
#include "QTextEdit"
#include "QTime"
#include "stdbool.h"
#include "QLCDNumber"
#include "mainwindow.h"
#include "EstacionDeServicio.h"
#include "JefeDeEstacion.h"
#include "Logger.h"
#include "Constantes.h"
#include "Marshaller.h"
#include "Constantes.h"
#include <sys/wait.h>

class WorkerThread : public QThread{
    Q_OBJECT

    void run() Q_DECL_OVERRIDE {

        MainWindow* mainWindow=MainWindow::getInstance();
        QLCDNumber* displayNumber=mainWindow->findChild<QLCDNumber*>("timeDisplay");

        int surtidores=mainWindow->getNumeroSurtidores();
        int empleados=mainWindow->getNumeroEmpleados();
        int tiempoSimulacion=mainWindow->getTiempoSimulacion();
        EstacionDeServicio::getInstancia()->setSurtidores(surtidores);
        JefeDeEstacion jefe;
        jefe.setEmpleados(empleados);
        Logger::debug(getpid(), "Inicio de simulacion\n");

         MemoriaCompartida<bool> juegoTerminado;
         juegoTerminado.crear("/bin/ls",1);
         juegoTerminado.escribir(false);

        pid_t pid = fork ();

        if ( pid == 0 ) {
            FifoLectura canal (FIFO_AUTOS);
            Marshaller marshaller;
            JefeDeEstacion jefe;

            while (!juegoTerminado.leer()){
                const int BUFFSIZE=100;
                char buffer[BUFFSIZE];
                canal.abrir();
                ssize_t bytesLeidos = canal.leer(static_cast<void*>(buffer),BUFFSIZE);
                std::string mensaje = buffer;
                mensaje.resize (bytesLeidos);
                Auto unAuto=marshaller.fromString(mensaje);
                cout<<"Leo auto: "<<unAuto.getCapacidad()<<endl;
                jefe.recibirAuto(unAuto);
                canal.cerrar();
            }
            juegoTerminado.liberar();
            exit (0);
        }else{

            QTime timeElapsed;
            timeElapsed.start();
            while (tiempoSimulacion > timeElapsed.elapsed()/1000) {
                if(timeElapsed.elapsed()%1000==0)
                    displayNumber->display(tiempoSimulacion - timeElapsed.elapsed()/1000);
            }
            juegoTerminado.escribir(true);
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
