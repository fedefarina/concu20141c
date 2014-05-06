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
#include "Semaforo.h"
#include "SigUnusedHandler.h"
#include "EventHandler.h"
#include "FifoLectura.h"
#include "SignalHandler.h"
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
        Semaforo semaforoFifo ( (char*)SEMAFORO_FIFO,0 );
        mainWindow->setSemaforoFifo(semaforoFifo);

        int surtidores=mainWindow->getNumeroSurtidores();
        int empleados=mainWindow->getNumeroEmpleados();
        int tiempoSimulacion=mainWindow->getTiempoSimulacion();
        if(surtidores<=0)
            cout<<"Debe haber al menos 1 surtidor."<<endl;
        if(empleados<=0)
            cout<<"Debe haber al menos 1 empleado."<<endl;

        if(surtidores>0 && empleados>0){

            restartUI(false);
            EstacionDeServicio estacion=EstacionDeServicio::getInstance();
            JefeDeEstacion jefe;
            estacion.setSurtidores(surtidores);
            estacion.setEmpleados(empleados);
            estacion.setJefeDeEstacion(jefe);


            QProgressBar* progressBar=mainWindow->findChild<QProgressBar*>("progressBar");
            Logger::debug(getpid(), "Inicio de simulacion\n");

            pid_t pid = fork ();
            if ( pid == 0 ) {

                FifoLectura canal (FIFO_AUTOS);
                canal.abrir();

                Marshaller marshaller;
                const int BUFFSIZE=100;
                char buffer[BUFFSIZE];

                SIGUNUSED_Handler sigint_handler;
                SignalHandler :: getInstance()->registrarHandler ( SIGUNUSED,&sigint_handler );
                while (sigint_handler.getGracefulQuit() == 0 ){
                    semaforoFifo.p();
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
                    progressBar->setValue((int) ((timeElapsed.elapsed())/tiempoSimulacion)/10);
                }
                if(timeElapsed.elapsed()%10==0)
                    QCoreApplication::processEvents();
            }

            progressBar->setValue(100);
            autosFifo.cerrar();
            autosFifo.eliminar();
            semaforoFifo.eliminar();
            kill(pid, SIGUNUSED);
            int estado;
            wait ((void *) &estado);
            onFinished();
        }
    }

private:
    void onFinished(){
        restartUI(true);
        Logger::debug(getpid(), string("Fin de simulacion\n"));
    }

    void restartUI(bool enabled){
        MainWindow* mainWindow=MainWindow::getInstance();
        QPushButton *ejecutarButton = mainWindow->findChild<QPushButton*>("iniciarButton");
        QPushButton *nuevoAutoButton = mainWindow->findChild<QPushButton*>("nuevoAuto");
        QProgressBar* progressBar=mainWindow->findChild<QProgressBar*>("progressBar");
        progressBar->setValue(100);
        ejecutarButton->setEnabled(enabled);
        nuevoAutoButton->setEnabled(!enabled);
    }

};
#endif // WORKERTHREAD_H
