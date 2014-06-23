#ifndef ADMINISTRADOR_H
#define ADMINISTRADOR_H

#include "Caja.h"

class Administrador{

private:
    Caja *caja;

public:
    Administrador();
    void consultarCaja();
    ~Administrador();
};

#endif // ADMINISTRADOR_H
