#include "FifoLectura.h"

FifoLectura::FifoLectura(const std::string nombre) : Fifo(nombre) {
}

FifoLectura::~FifoLectura() {
}

void FifoLectura::abrir() {
    fd = open ( nombre.c_str(),O_RDONLY,0);
}

ssize_t FifoLectura::leer(void* buffer,const ssize_t buffsize) const {
	return read ( fd,buffer,buffsize );
}
