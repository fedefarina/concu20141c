#ifndef SEMAFORO_H_
#define SEMAFORO_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include "Logger.h"

class Semaforo {

private:
    int id;
    int inicializar (int valorInicial, unsigned int posicion);

public:
    Semaforo();
    explicit Semaforo (char* nombre,unsigned int cantidad=1);
    explicit Semaforo (char* nombre, unsigned int valorInicial, unsigned int cantidad);
    virtual ~Semaforo();

    int p (unsigned int posicion=0); // decrementa
    int v (unsigned int posicion=0); // incrementa

    void eliminar ();
    int getValue(unsigned int posicion=0) const;
};

#endif /* SEMAFORO_H_ */
