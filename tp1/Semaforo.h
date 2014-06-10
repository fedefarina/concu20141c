#ifndef SEMAFORO_H_
#define SEMAFORO_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

class Semaforo {

private:
    int id;
    int inicializar (int valorInicial, unsigned int posicion);

public:
    Semaforo();
    explicit Semaforo (char* nombre,unsigned int cantidad=1);
    explicit Semaforo (char* nombre, int valorInicial, unsigned int cantidad=1);
    virtual ~Semaforo();

    int p (unsigned int posicion=0); // decrementa
    int v (unsigned int posicion=0); // incrementa

    void eliminar ();
    int getValue(unsigned int posicion) const;
};

#endif /* SEMAFORO_H_ */
