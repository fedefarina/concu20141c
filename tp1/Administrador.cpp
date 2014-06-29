#include "Administrador.h"
#include "Caja.h"

Administrador::Administrador(){
    caja=new Caja();
}

void Administrador::consultarCaja(){
    unsigned int saldo=caja->consultarSaldo();
    Utils<unsigned int> utils;
    Logger::debug(getpid(),"Saldo:"+ utils.toString(saldo) +"\n");
}

Administrador::~Administrador(){
    delete(caja);
}
