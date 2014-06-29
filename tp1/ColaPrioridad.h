#ifndef COLA_H_
#define COLA_H_

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string>

template <class T> class ColaPrioridad {
private:
    key_t	clave;
    int		id;

public:
    ColaPrioridad ( const std::string& archivo,const char letra );
    ~ColaPrioridad();
    int escribir ( const T& dato ) const;
    int leer (T* buffer ) const;
    int destruir () const;
};

template <class T> ColaPrioridad<T> :: ColaPrioridad ( const std::string& archivo,const char letra ) {
    this->clave = ftok ( archivo.c_str(),letra );
    if ( this->clave == -1 )
        perror ( "Error en ftok" );

    this->id = msgget ( this->clave,0777|IPC_CREAT );
    if ( this->id == -1 )
        perror ( "Error en msgget" );
}

template <class T> ColaPrioridad<T> :: ~ColaPrioridad () {
}

template <class T> int ColaPrioridad<T> :: destruir () const {
    int resultado = msgctl ( this->id,IPC_RMID,NULL );
    return resultado;
}

template <class T> int ColaPrioridad<T> :: escribir ( const T& dato ) const {
    int resultado = msgsnd ( this->id,static_cast<const void*>(&dato),sizeof(T)-sizeof(long),0 );
    return resultado;
}

template <class T> int ColaPrioridad<T> :: leer (T* buffer ) const {
    int resultado = msgrcv ( this->id,static_cast<void *>(buffer),sizeof(T)-sizeof(long),-10,0 );
    if(resultado!=-1)
        std::cout<<"Bien"<<std::endl;

    return resultado;
}

#endif /* COLA_H_ */
