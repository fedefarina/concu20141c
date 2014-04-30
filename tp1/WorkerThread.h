#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H
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

class WorkerThread{

public:
    WorkerThread(){}
    ~WorkerThread(){}
    void run(){

        MainWindow* mainWindow=MainWindow::getInstance();

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
                canal.abrir();

                Marshaller marshaller;
                const int BUFFSIZE=100;
                char buffer[BUFFSIZE];

                while (!juegoTerminado.leer()){
                    cout << "Leo" << endl;
                    ssize_t bytesLeidos = canal.leer(static_cast<void*>(buffer),BUFFSIZE);
                    if(bytesLeidos>0){
                        std::string mensaje = buffer;
                        mensaje.resize (bytesLeidos);

                        Auto unAuto=marshaller.fromString(mensaje);
                        jefe.recibirAuto(unAuto);
                    }
                }
                canal.cerrar();
                canal.eliminar();
                cout << "Hijo: Terrmine" << endl;
                exit (0);
            }


            FifoEscritura autosFifo(FIFO_AUTOS);
            autosFifo.abrir();
            mainWindow->setAutosFifo(autosFifo);
            QTime timeElapsed;
            timeElapsed.start();

            while (tiempoSimulacion > timeElapsed.elapsed()/1000) {
                if(timeElapsed.elapsed()%1000==0){
                    displayNumber->display(tiempoSimulacion - timeElapsed.elapsed()/1000);
                }
                QCoreApplication::processEvents();
            }


            autosFifo.cerrar();
            autosFifo.eliminar();
            juegoTerminado.escribir(true);
            int estado;
            wait ( (void*) &estado );
            juegoTerminado.liberar();
            displayNumber->display(0);
            onFinished();
        }
    }

private:
    void onFinished(){
        EstacionDeServicio::destruirInstancia();
        enableButtons(true);
        Logger::debug(getpid(), string("Fin de simulacion\n"));
    }

    void enableButtons(bool enabled){
        MainWindow* mainWindow=MainWindow::getInstance();
        QPushButton *ejecutarButton = mainWindow->findChild<QPushButton*>("iniciarButton");
        QPushButton *nuevoAutoButton = mainWindow->findChild<QPushButton*>("nuevoAuto");
        ejecutarButton->setEnabled(enabled);
        nuevoAutoButton->setEnabled(!enabled);
    }

};
#endif // WORKERTHREAD_H
