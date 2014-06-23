#include "Administrador.h"
#include "Caja.h"

Administrador::Administrador(){
    caja=new Caja();
}

void Administrador::consultarCaja(){
    Logger::debug(getpid(),"El administrador usa la caja\n");
    unsigned int saldo=caja->consultarSaldo();
    Utils<unsigned int> utils;
    Logger::debug(getpid(),"Saldo:"+ utils.toString(saldo) +"\n");
    Logger::debug(getpid(),"El administrador termino de usar la caja\n");
}

Administrador::~Administrador(){
    delete(caja);
}
