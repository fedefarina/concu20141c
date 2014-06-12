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
#include "MemoriaCompartida.h"
#include "Semaforo.h"
#include "Cola.h"
#include "Mensajes.h"

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
    void cleanInputVariables();
    bool event(QEvent *event);
    void iniciarSimulacion();
    void finalizarSimulacion();
    ~MainWindow();

public slots:
    void nuevoAuto();
    void nuevoAutoVip();
    void ejecutarComando();
    void getSaldo();

private:
    int nSurtidores, nEmpleados, tiempoSimulacion;
    Cola<mensaje> *colaAutos;
    Semaforo semaforoCola;
    Semaforo semaforoCaja;
    MemoriaCompartida<short int> caja;
    Ui::MainWindow *ui;
    explicit MainWindow(QWidget *parent = 0);
    static MainWindow* instance;

};


#endif // MAINWINDOW_H
