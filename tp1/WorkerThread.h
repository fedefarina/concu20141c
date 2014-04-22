#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H
#include "QThread"
#include "QPushButton"
#include "QTextEdit"
#include "mainwindow.h"

class WorkerThread : public QThread{
    Q_OBJECT

    void run() Q_DECL_OVERRIDE {

        MainWindow* mainWindow=MainWindow::getInstance();
        //Para que la vista no se ponga unresponsive mantenemos la vista en un proceso separado.
        //int surtidores=mainWindow->getNumeroSurtidores();
        //int empleados=mainWindow->getNumeroSurtidores();
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
