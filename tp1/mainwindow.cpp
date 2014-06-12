#include <qtextedit.h>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QFileDialog>
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "Main.h"
#include "Constantes.h"
#include <sys/wait.h>

MainWindow* MainWindow::instance = NULL;


MainWindow* MainWindow::getInstance() {
    if (instance == NULL)
        instance= new MainWindow();
    return instance;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);
    QPushButton *ejecutarButton = this->findChild<QPushButton*>("iniciarButton");
    QPushButton *nuevoAutoButton = this->findChild<QPushButton*>("nuevoAutoButton");
    QPushButton *saldoButton = this->findChild<QPushButton*>("saldoButton");

    QLineEdit* capacidadEdit=this->findChild<QLineEdit*>("capacidadEdit");
    capacidadEdit->setValidator(new QIntValidator(this));

    //sender, signal, receiver, slot (callback similar)
    QObject::connect(ejecutarButton, SIGNAL(clicked()), this, SLOT(ejecutarComando()));
    QObject::connect(saldoButton, SIGNAL(clicked()), this, SLOT(getSaldo()));
    QObject::connect(nuevoAutoButton, SIGNAL(clicked()), this, SLOT(nuevoAuto()));
}


void MainWindow::ejecutarComando(){
    QString surtidores=this->findChild<QSpinBox*>("nSurtidores")->text();
    QString empleados=this->findChild<QSpinBox*>("nEmpleados")->text();
    QTimeEdit *timeEdit=this->findChild<QTimeEdit*>("timeEdit");
    QString tSimulacion=timeEdit->text();
    nSurtidores=surtidores.toInt();
    nEmpleados=empleados.toInt();
    tiempoSimulacion=tSimulacion.toInt();

    Main *workerThread = new Main();
    workerThread->run();
}


void MainWindow::iniciarSimulacion(){

    Cola<mensaje> *cola =new Cola<mensaje>( COLA_MENSAJES,'C');
    this->colaAutos=cola;

    this->caja.crear((char*)MEMORIA_CAJA, 'C');

    Semaforo semaforoCola((char*) SEMAFORO_COLA);
    Semaforo semaforoCaja((char*) SEMAFORO_CAJA);
    this->semaforoCola=semaforoCola;
    this->semaforoCaja=semaforoCaja;
}

void MainWindow::finalizarSimulacion(){
    this->caja.liberar();
    delete(colaAutos);
}

void MainWindow::getSaldo(){
    QLineEdit* saldoEdit=this->findChild<QLineEdit*>("saldoEdit");
    saldoEdit->setEnabled(false);
    this->semaforoCaja.p();
    unsigned int saldo=this->caja.leer();
    this->semaforoCaja.v();
    Utils<unsigned int> utils;
    saldoEdit->setText(utils.toString(saldo).c_str());
    saldoEdit->setEnabled(true);
}

void MainWindow::nuevoAuto(){
    Marshaller marshaller;
    Auto unAuto;
    int capacidad=this->findChild<QLineEdit*>("capacidadEdit")->text().toInt();
    unAuto.setCapacidad(capacidad);
    mensaje msg;
    msg.mtype=1;
    msg.capacidad=unAuto.getCapacidad();
    std::cout<<"mainwindows p value"<<semaforoCola.getValue()<<std::endl;

    //semaforoCola.p();
    colaAutos->escribir(msg);
    semaforoCola.v2();
    std::cout<<"Agregue un nuevo auto"<<std::endl;
}

void MainWindow::nuevoAutoVip(){
    Marshaller marshaller;
    Auto unAuto;
    int capacidad=this->findChild<QLineEdit*>("capacidadEdit")->text().toInt();
    unAuto.setCapacidad(capacidad);
    mensaje msg;
    msg.mtype=0;
    msg.capacidad=unAuto.getCapacidad();
    //semaforoCola.p();
    colaAutos->escribir(msg);
    semaforoCola.v2();
}


bool MainWindow::event(QEvent *event){
    if (event->type() == QEvent::KeyPress){
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Enter || ke->key() == Qt::Key_Return){
            QPushButton *ejecutarButton = this->findChild<QPushButton*>("iniciarButton");
            ejecutarButton->animateClick();
            return true;
        }
    }
    return QMainWindow::event(event);
}

int MainWindow::getNumeroSurtidores(){
    return nSurtidores;
}

int MainWindow::getNumeroEmpleados(){
    return nEmpleados;
}

int MainWindow::getTiempoSimulacion(){
    return tiempoSimulacion;
}

MainWindow::~MainWindow(){
    delete ui;
}

