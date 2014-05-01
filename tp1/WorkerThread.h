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
#include "QProgressBar"
#include <sys/wait.h>

/**
 * @brief The WorkerThread class
 * Maneja el loop principal de la simulacion.
 */

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

            EstacionDeServicio::getInstance().setSurtidores(surtidores);

            QProgressBar* progressBar=mainWindow->findChild<QProgressBar*>("progressBar");

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

                cout << "Hijo: Termine" << endl;

                exit (0);
            }


            FifoEscritura autosFifo(FIFO_AUTOS);
            autosFifo.abrir();
            mainWindow->setAutosFifo(autosFifo);
            QTime timeElapsed;
            timeElapsed.start();

            while (tiempoSimulacion > timeElapsed.elapsed()/1000) {
                if(timeElapsed.elapsed()%1000==0){
                    displayNumber->display(timeElapsed.elapsed()/1000);
                    progressBar->setValue(100- (int) ((timeElapsed.elapsed())/tiempoSimulacion)/10);
                }
                if(timeElapsed.elapsed()%10==0)
                    QCoreApplication::processEvents();
            }

            progressBar->setValue(0);
            displayNumber->display(tiempoSimulacion);

            autosFifo.cerrar();
            autosFifo.eliminar();
            juegoTerminado.escribir(true);
            int estado;
            wait ( (void*) &estado );
            juegoTerminado.liberar();
            onFinished();
        }
    }

private:
    void onFinished(){
        //EstacionDeServicio::destruirInstancia();
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
