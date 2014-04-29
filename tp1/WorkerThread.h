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

        FifoEscritura autosFifo(FIFO_AUTOS);
        MainWindow* mainWindow=MainWindow::getInstance();
        mainWindow->setAutosFifo(autosFifo);
        int surtidores=mainWindow->getNumeroSurtidores();
        int empleados=mainWindow->getNumeroEmpleados();
        int tiempoSimulacion=mainWindow->getTiempoSimulacion();

        if(surtidores<=0)
            cout<<"Debe haber al menos 1 surtidor."<<endl;
        if(empleados<=0)
            cout<<"Debe haber al menos 1 empleado."<<endl;

        if(surtidores>0 && empleados>0){

            enableButtons(false);

            QLCDNumber* displayNumber=mainWindow->findChild<QLCDNumber*>("timeDisplay");
            EstacionDeServicio::getInstancia()->setSurtidores(surtidores);
            JefeDeEstacion jefe;
            jefe.setEmpleados(empleados);
            Logger::debug(getpid(), "Inicio de simulacion\n");

            MemoriaCompartida<bool> juegoTerminado;
            juegoTerminado.crear("/bin/ls",'A');
            juegoTerminado.escribir(false);

            pid_t pid = fork ();

            if ( pid == 0 ) {
                FifoLectura canal (FIFO_AUTOS);
                Marshaller marshaller;

                while (!juegoTerminado.leer()){
                    const int BUFFSIZE=100;
                    char buffer[BUFFSIZE];
                    canal.abrir();
                    ssize_t bytesLeidos = canal.leer(static_cast<void*>(buffer),BUFFSIZE);
                    std::string mensaje = buffer;
                    mensaje.resize (bytesLeidos);
                    Auto unAuto=marshaller.fromString(mensaje);
                    jefe.recibirAuto(unAuto);
                    canal.cerrar();
                }
                exit (0);
            }else{
                QTime timeElapsed;
                timeElapsed.start();
                while (tiempoSimulacion > timeElapsed.elapsed()/1000) {
                    if(timeElapsed.elapsed()%1000==0)
                        displayNumber->display(tiempoSimulacion - timeElapsed.elapsed()/1000);
                }
                juegoTerminado.escribir(true);
                displayNumber->display(0);
                juegoTerminado.liberar();
                autosFifo.eliminar();
                emit finishSignal();
            }
        }
    }

signals:
    void finishSignal();
public slots:
    void onFinished(){
        EstacionDeServicio::destruirInstancia();
        enableButtons(true);
        Logger::debug(getpid(), string("Fin de simulacion\n"));
    }
public:
    void enableButtons(bool enabled){
        MainWindow* mainWindow=MainWindow::getInstance();
        QPushButton *ejecutarButton = mainWindow->findChild<QPushButton*>("iniciarButton");
        QPushButton *nuevoAutoButton = mainWindow->findChild<QPushButton*>("nuevoAuto");

        ejecutarButton->setEnabled(enabled);
        nuevoAutoButton->setEnabled(!enabled);
    }

};
#endif // WORKERTHREAD_H
