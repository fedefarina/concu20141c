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
    explicit Semaforo (char* nombre);
    explicit Semaforo (char* nombre, int valorInicial, unsigned int cantidad);
    virtual ~Semaforo();

    int p (unsigned int posicion); // decrementa
    int v (unsigned int posicion); // incrementa

    void eliminar ();
    int getValue(unsigned int posicion) const;
};

#endif /* SEMAFORO_H_ */
