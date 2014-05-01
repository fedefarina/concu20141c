#include <qtextedit.h>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QFileDialog>
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "WorkerThread.h"
#include "Constantes.h"
#include "QCommandLinkButton"
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
    QCommandLinkButton *nuevoAutoButton = this->findChild<QCommandLinkButton*>("nuevoAuto");

    QLineEdit* capacidadEdit=this->findChild<QLineEdit*>("capacidadEdit");
    capacidadEdit->setValidator(new QIntValidator(this));

    //sender, signal, receiver, slot (callback similar)
    QObject::connect(ejecutarButton, SIGNAL(clicked()), this, SLOT(ejecutarComando()));
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

    WorkerThread *workerThread = new WorkerThread();
    workerThread->run();
}

void MainWindow::setAutosFifo(const FifoEscritura& fifoAutos){
    this->fifoAutos=fifoAutos;
}

void MainWindow::nuevoAuto(){

    Marshaller marshaller;
    Auto unAuto;
    int capacidad=this->findChild<QLineEdit*>("capacidadEdit")->text().toInt();
    unAuto.setCapacidad(capacidad);
    std::string mensaje=marshaller.toString(unAuto);
    pid_t pid = fork ();
    if ( pid == 0 ) {//Hijo
        fifoAutos.escribir ( static_cast<const void*>(mensaje.c_str()),mensaje.length() );
        exit(0);
    }else{
        int estado;
        wait ( (void*) &estado );
    }
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

