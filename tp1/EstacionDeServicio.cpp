#include "EstacionDeServicio.h"

EstacionDeServicio EstacionDeServicio::instance;

EstacionDeServicio::EstacionDeServicio() {

}

EstacionDeServicio::~EstacionDeServicio() {
    this->surtidores.liberar();
}

EstacionDeServicio& EstacionDeServicio::getInstance() {
    //if (instancia == NULL)
        //instancia = new EstacionDeServicio();
    return instance;
}
/*
void EstacionDeServicio::destruirInstancia() {
    if (instancia != NULL){
        delete (instancia);
        instancia=NULL;
    }
}
*/
void EstacionDeServicio::setSurtidores(unsigned int surtidores) {
    this->surtidores.crear((char*)"/bin/ls", 34, surtidores);

    for(unsigned int i = 0; i < surtidores; i++) {
        this->surtidores.escribir(true, i);
    }
}

unsigned int EstacionDeServicio::getSurtidores() {
    return this->surtidores.cantidad();
}
