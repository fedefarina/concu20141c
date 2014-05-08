#ifndef SEMAFORO_H_
#define SEMAFORO_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

class Semaforo {

private:
    int id;
    int inicializar (int valorInicial);

public:
    Semaforo();
    explicit Semaforo (char* nombre);
    explicit Semaforo ( char* nombre,int valorInicial );
    virtual ~Semaforo();

    int p (); // decrementa
    int v (); // incrementa
    void eliminar ();
    int getValue() const;
};

#endif /* SEMAFORO_H_ */
