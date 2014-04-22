#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "WorkerThread.h"

#include <qtextedit.h>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QFileDialog>

MainWindow* MainWindow::instance = NULL;


MainWindow* MainWindow::getInstance() {
    if (instance == NULL)
        instance= new MainWindow();
    return instance;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    QPushButton *ejecutarButton = this->findChild<QPushButton*>("iniciarButton");

    //sender, signal, receiver, slot (callback similar)
    QObject::connect(ejecutarButton, SIGNAL(clicked()), this, SLOT(ejecutarComando()));
    QObject::connect(this, SIGNAL(updateSalidaSignal(QString)), this, SLOT(updateSalida(QString)));
}


void MainWindow::ejecutarComando(){

    writeToStdOuT("Iniciando simulación");
    QPushButton *ejecutarButton = this->findChild<QPushButton*>("iniciarButton");
    ejecutarButton->setEnabled(false);


    WorkerThread *workerThread = new WorkerThread();
    connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
    connect(workerThread, SIGNAL(finishSignal()), workerThread, SLOT(onFinished()));
    workerThread->start();

}


//Si tengo el boton onFocus con el enter inició la aplicación.
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


void MainWindow::writeToStdOuT(const string mje){
    QString qstr = QString::fromStdString(mje);
    emit updateSalidaSignal(qstr);
}

void MainWindow::updateSalida(const QString &s){
    QTextEdit* salidaText = this->findChild<QTextEdit*>("salidaEditText");
    salidaText->append(s);
}

int MainWindow::getNumeroSurtidores(){
    return nSurtidores;
}

int MainWindow::getNumeroEmpleados(){
    return nEmpleados;
}

MainWindow::~MainWindow(){
    delete ui;
}

