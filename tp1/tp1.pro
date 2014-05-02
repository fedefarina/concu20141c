QT       += core gui
QT       += widgets

HEADERS += \
    MemoriaCompartida.h \
    WorkerThread.h \
    mainwindow.h \
    Logger.h \
    LockFile.h \
    JefeDeEstacion.h \
    Empleado.h \
    Auto.h \
    EstacionDeServicio.h \
    FifoLectura.h \
    FifoEscritura.h \
    Fifo.h \
    Marshaller.h \
    Utils.h \
    Constantes.h \
    Semaforo.h \
    SignalHandler.h \
    EventHandler.h \
    SigUnusedHandler.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    Logger.cpp \
    LockFile.cpp \
    EstacionDeServicio.cpp \
    FifoLectura.cpp \
    FifoEscritura.cpp \
    Fifo.cpp \
    Semaforo.cpp \
    SignalHandler.cpp

FORMS += \
    mainwindow.ui \
