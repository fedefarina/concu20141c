#include <QApplication>
#include "mainwindow.h"

using namespace std;

int main(int argc, char **argv) {

    QApplication a(argc, argv);
    MainWindow* w=MainWindow::getInstance();
    w->show();
    return a.exec();
    delete(w);
    return 0;
}

