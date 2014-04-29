#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <QMainWindow>
#include <stdio.h>
#include <string>
#include "FifoEscritura.h"

namespace Ui {
    class MainWindow;
}

using namespace std;

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    static MainWindow* getInstance();
    int getNumeroSurtidores();
    int getNumeroEmpleados();
    int getTiempoSimulacion();
    void setAutosFifo(const FifoEscritura fifoAutos);
    void cleanInputVariables();
    bool event(QEvent *event);
    ~MainWindow();

public slots:
    void nuevoAuto();
    void ejecutarComando();

private:
    int nSurtidores, nEmpleados, tiempoSimulacion;
    FifoEscritura fifoAutos;
    Ui::MainWindow *ui;
    explicit MainWindow(QWidget *parent = 0);
    static MainWindow* instance;

};


#endif // MAINWINDOW_H
