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
    QPushButton *nuevoAutoVipButton = this->findChild<QPushButton*>("nuevoAutoVipButton");
    QPushButton *saldoButton = this->findChild<QPushButton*>("saldoButton");

    QLineEdit* capacidadEdit=this->findChild<QLineEdit*>("capacidadEdit");
    capacidadEdit->setValidator(new QIntValidator(this));

    QLineEdit* capacidadVipEdit=this->findChild<QLineEdit*>("capacidadVipEdit");
    capacidadVipEdit->setValidator(new QIntValidator(this));

    //sender, signal, receiver, slot (callback similar)
    QObject::connect(ejecutarButton, SIGNAL(clicked()), this, SLOT(ejecutarComando()));
    QObject::connect(saldoButton, SIGNAL(clicked()), this, SLOT(notificarPeticion()));
    QObject::connect(nuevoAutoButton, SIGNAL(clicked()), this, SLOT(recibirAuto()));
    QObject::connect(nuevoAutoVipButton, SIGNAL(clicked()), this, SLOT(recibirAutoVip()));
}


void MainWindow::ejecutarComando(){
    QString surtidores=this->findChild<QSpinBox*>("nSurtidores")->text();
    QString empleados=this->findChild<QSpinBox*>("nEmpleados")->text();
    QTimeEdit *timeEdit=this->findChild<QTimeEdit*>("timeEdit");
    QTime time=timeEdit->time();
    tiempoSimulacion=time.minute()*60+time.second();

    nSurtidores=surtidores.toInt();
    nEmpleados=empleados.toInt();

    Main *workerThread = new Main();
    workerThread->run();
}


void MainWindow::iniciarSimulacion(){

    Cola<mensaje> *colaAutos = new Cola<mensaje>( COLA_AUTOS,'C');
    this->colaAutos=colaAutos;
    this->caja = new Caja();
    Semaforo semaforoCola((char*) SEMAFORO_COLA_AUTOS);
    this->semaforoColaAutos=semaforoCola;

    Semaforo semaforoColaCaja((char*) SEMAFORO_COLA_CAJA);
    this->semaforoColaCaja=semaforoColaCaja;

    Cola<mensaje> *colaCaja =new Cola<mensaje>( COLA_CAJA,'C');
    this->colaCaja=colaCaja;
}

void MainWindow::finalizarSimulacion(){
    delete(colaAutos);
    delete(caja);
    delete(colaCaja);
}

void MainWindow::notificarPeticion(){
    Logger::debug(getpid(),"El administrador pide usar la caja\n");
    mensaje msg;
    msg.mtype=ADMINISTRADOR;
    semaforoColaCaja.p();
    colaCaja->escribir(msg);
    semaforoColaCaja.v();
}

void MainWindow::recibirAuto(){
    nuevoAuto(AUTO);
}

void MainWindow::recibirAutoVip(){
    nuevoAuto(AUTO_VIP);
}

void MainWindow::nuevoAuto(unsigned int tipo){

    JefeDeEstacion jefe;
    string tipoAuto=(tipo==AUTO_VIP)?" VIP":"";
    if(jefe.getEmpleadosOcupados()==nEmpleados){
        Logger::debug(getpid(), "Evento > No hay empleados disponibles\n");
        Logger::debug(getpid(), "Evento > El auto"+ tipoAuto +" se retira de la estación de servicio\n");
        return;
    }else{
        Logger::debug(getpid(),"Evento > Un nuevo auto"+tipoAuto+" entra a la estacion de servicio\n",true);
    }

    int capacidad;

    if(tipo==AUTO){
        capacidad=this->findChild<QLineEdit*>("capacidadEdit")->text().toInt();
    }else{
        capacidad=this->findChild<QLineEdit*>("capacidadVipEdit")->text().toInt();
    }

    mensaje msg;
    msg.mtype=tipo;
    msg.capacidad=capacidad;
    semaforoColaAutos.p();
    colaAutos->escribir(msg);
    semaforoColaAutos.v();
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

