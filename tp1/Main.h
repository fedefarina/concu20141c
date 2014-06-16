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
#include "QProgressBar"
#include "SignalEventHandler.h"
#include "EventHandler.h"
#include "SignalHandler.h"
#include "QCheckBox"
#include <sys/wait.h>


/**
 * @brief The Main class
 * Maneja el loop principal de la simulacion.
 */

class Main{

public:
    Main(){}
    ~Main(){}

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

            restartUI(false);
            QCheckBox* debugCheckBox=mainWindow->findChild<QCheckBox*>("debugCheckBox");
            Logger::setDebugMode(debugCheckBox->isChecked());

            EstacionDeServicio::getInstancia()->setEmpleados(empleados);
            EstacionDeServicio::getInstancia()->setSurtidores(surtidores);

            Cola<mensaje> *cola =new Cola<mensaje>( COLA_AUTOS,'C');
            QProgressBar* progressBar=mainWindow->findChild<QProgressBar*>("progressBar");
            Logger::debug(getpid(), "Inicio de simulacion\n");
            QTime timeElapsed;
            timeElapsed.start();

            JefeDeEstacion* jefe = new JefeDeEstacion();

            SignalEventHandler sigUnusedHandler;
            SignalHandler :: getInstance()->registrarHandler ( SIGUNUSED,&sigUnusedHandler );

            pid_t pid = fork ();
            if ( pid == 0 ) {
                while (sigUnusedHandler.getGracefulQuit() == 0 ){
                    jefe->recibirAuto();
                    jefe->recibirPeticionesCaja();
                }

                while(jefe->getEmpleadosOcupados()>0);
                delete(jefe);
                exit (0);
            }

            mainWindow->iniciarSimulacion();
            SignalEventHandler sigAbortHandler;
            SignalHandler :: getInstance()->registrarHandler ( SIGABRT,&sigAbortHandler );


            while (tiempoSimulacion > timeElapsed.elapsed()/1000) {

                if(timeElapsed.elapsed()%1000==0){
                    progressBar->setValue((int) ((timeElapsed.elapsed())/tiempoSimulacion)/10);
                }

                if(timeElapsed.elapsed()%10==0)
                    QCoreApplication::processEvents();

                //Actualizo la vista cuando el administrador termina de consular la caja
                if(sigAbortHandler.getGracefulQuit()==1){
                    Logger::debug(getpid(), "Actualizando\n");
                    sigAbortHandler.reset();
                    mainWindow->mostrarSaldo();
                }
            }

            progressBar->setValue(100);
            MainWindow* mainWindow=MainWindow::getInstance();
            disableAutoButton();


            kill(pid, SIGUNUSED);

            while (jefe->getEmpleadosOcupados()>0)
                QCoreApplication::processEvents();

            int estado;
            wait ((void *) &estado);
            mainWindow->finalizarSimulacion();
            onFinished();
            delete(jefe);
            delete(cola);
            EstacionDeServicio::destruirInstancia();
        }
    }

private:
    void onFinished(){
        restartUI(true);
        Logger::debug(getpid(), string("Fin de simulacion\n"));
    }

    void disableAutoButton(){
        MainWindow* mainWindow=MainWindow::getInstance();
        QPushButton *nuevoAutoButton = mainWindow->findChild<QPushButton*>("nuevoAutoButton");
        QPushButton *nuevoAutoVipButton = mainWindow->findChild<QPushButton*>("nuevoAutoVipButton");
        nuevoAutoButton->setEnabled(false);
        nuevoAutoVipButton->setEnabled(false);
    }

    void restartUI(bool enabled){
        MainWindow* mainWindow=MainWindow::getInstance();
        QPushButton *ejecutarButton = mainWindow->findChild<QPushButton*>("iniciarButton");
        QPushButton *nuevoAutoButton = mainWindow->findChild<QPushButton*>("nuevoAutoButton");
        QPushButton *nuevoAutoVipButton = mainWindow->findChild<QPushButton*>("nuevoAutoVipButton");
        QPushButton *saldoButton = mainWindow->findChild<QPushButton*>("saldoButton");
        saldoButton->setEnabled(!enabled);
        ejecutarButton->setEnabled(enabled);
        nuevoAutoButton->setEnabled(!enabled);
        nuevoAutoVipButton->setEnabled(!enabled);
    }

};
#endif // WORKERTHREAD_H
