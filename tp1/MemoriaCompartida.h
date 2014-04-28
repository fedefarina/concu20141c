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


template <class T> class MemoriaCompartida {

private:

    int 	shmId;
    T*		ptrDatos;
    unsigned int cantidad;
    int cantidadProcesosAdosados ();

public:

    MemoriaCompartida ();
    ~MemoriaCompartida ();
    int crear ( std::string archivo, char letra, int cantidad = 1 );
    void liberar ();
    void escribir ( T dato, int posicion = 0 );
    T leer ( int posicion = 0 );
    unsigned int getCantidad ();

};

template <class T> MemoriaCompartida<T> :: MemoriaCompartida () {
    this->shmId = 0;
    this->ptrDatos = NULL;
}

template <class T> MemoriaCompartida<T> :: ~MemoriaCompartida () {
}

template <class T> int MemoriaCompartida<T> :: crear ( std::string archivo, char letra, int cantidad ) {


    // generacion de la clave
    key_t clave = ftok ( archivo.c_str(),letra );
    if ( clave == -1 )
        return ERROR_FTOK;
    else {
        // creacion de la memoria compartida
        this->shmId = shmget ( clave,sizeof(T)*cantidad,0644|IPC_CREAT );

        if ( this->shmId == -1 )
            return ERROR_SHMGET;
        else {
            // attach del bloque de memoria al espacio de direcciones del proceso
            void* ptrTemporal = shmat ( this->shmId,NULL,0 );

            if ( ptrTemporal == (void *) -1 ) {
                return ERROR_SHMAT;
            } else {                
                this->ptrDatos = (T *) ptrTemporal;
                this->cantidad = cantidad;
                return SHM_OK;
            }
        }
    }
}


template <class T> void MemoriaCompartida<T> :: liberar () {
    // detach del bloque de memoria
    shmdt ( (void *) this->ptrDatos );

    int procAdosados = this->cantidadProcesosAdosados ();

    if ( procAdosados == 0 ) {
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

template <class T> unsigned int MemoriaCompartida<T> :: getCantidad () {
    return this->cantidad;
}


#endif /* MEMORIACOMPARTIDA_H_ */
