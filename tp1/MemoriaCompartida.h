#ifndef MEMORIACOMPARTIDA_H_
#define MEMORIACOMPARTIDA_H_

#define SHM_OK			0
#define	ERROR_FTOK		-1
#define ERROR_SHMGET	-2
#define	ERROR_SHMAT		-3

#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>
#include	<string>
#include "Logger.h"
#include "Utils.h"


template <class T> class MemoriaCompartida {

private:

    int 	shmId;
    T*		ptrDatos;
    unsigned int numero;
    int cantidadProcesosAdosados ();

public:

    MemoriaCompartida ();
    ~MemoriaCompartida ();
    int crear ( std::string archivo, char letra, int numero = 1 );
    void liberar ();
    void escribir ( T dato, int posicion = 0 );
    T leer ( int posicion = 0 );
    unsigned int cantidad ();

};

template <class T> MemoriaCompartida<T> :: MemoriaCompartida () {
    this->shmId = 0;
    this->ptrDatos = NULL;
}

template <class T> MemoriaCompartida<T> :: ~MemoriaCompartida () {
}

template <class T> int MemoriaCompartida<T> :: crear ( std::string archivo, char letra, int numero ) {


    // generacion de la clave
    key_t clave = ftok ( archivo.c_str(),letra );
    if ( clave == -1 )
        return ERROR_FTOK;
    else {
        // creacion de la memoria compartida
        this->shmId = shmget ( clave,sizeof(T)*numero,0644|IPC_CREAT );

        if ( this->shmId == -1 )
            return ERROR_SHMGET;
        else {
            // attach del bloque de memoria al espacio de direcciones del proceso
            void* ptrTemporal = shmat ( this->shmId,NULL,0 );

            if ( ptrTemporal == (void *) -1 ) {
                return ERROR_SHMAT;
            } else {
                this->ptrDatos = (T *) ptrTemporal;
                this->numero = numero;
                return SHM_OK;
            }
        }
    }
}


template <class T> void MemoriaCompartida<T> :: liberar () {
    // detach del bloque de memoria
    shmdt ( (void *) this->ptrDatos );


    Utils<int> utils;
    string str = utils.toString(this->cantidadProcesosAdosados());
    str += "\n";
    Logger::debug(getpid(), str);
    int procAdosados = this->cantidadProcesosAdosados ();
    std::cout << "Proc Adosados: "<<procAdosados<<std::endl;

    if ( procAdosados == 0 ) {
        std::cout << "Termino bien: "<<procAdosados<<std::endl;
        Logger::debug(getpid(), "Termino bien\n");
        shmctl ( this->shmId,IPC_RMID,NULL );
    }
}

template <class T> void MemoriaCompartida<T> :: escribir ( T dato, int posicion ) {
    this->ptrDatos[posicion] = dato;
}

template <class T> T MemoriaCompartida<T> :: leer ( int posicion ) {
    return ( this->ptrDatos[posicion] );
}

template <class T> int MemoriaCompartida<T> :: cantidadProcesosAdosados () {
    shmid_ds estado;
    shmctl ( this->shmId,IPC_STAT,&estado );
    return estado.shm_nattch;
}

template <class T> unsigned int MemoriaCompartida<T> :: cantidad () {
    return this->numero;
}


#endif /* MEMORIACOMPARTIDA_H_ */
