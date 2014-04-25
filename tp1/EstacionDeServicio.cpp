#include "EstacionDeServicio.h"

EstacionDeServicio* EstacionDeServicio::instancia = NULL;

EstacionDeServicio::EstacionDeServicio() {

}

EstacionDeServicio::~EstacionDeServicio() {
    this->surtidores.liberar();
}

EstacionDeServicio* EstacionDeServicio::getInstancia() {
    if (instancia == NULL)
        instancia = new EstacionDeServicio();
    return instancia;
}

void EstacionDeServicio::destruirInstancia() {
    if (instancia != NULL)
        delete (instancia);
}

void EstacionDeServicio::setSurtidores(unsigned int surtidores) {
    this->surtidores.crear((char*)"/bin/ls", 34, surtidores);

    for(unsigned int i = 0; i < surtidores; i++) {
        this->surtidores.escribir(true, i);
    }
}

unsigned int EstacionDeServicio::getSurtidores() {
    return this->surtidores.getCantidad();
}
