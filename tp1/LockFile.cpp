#include "LockFile.h"

/**
 * @brief LockFile::LockFile
 *
 * Uso este archivo para sincronizar la lectura en el logger
 *
 * @param nombre del archivo que bloqueo
 *
 * SETLK:
 *  *  Intento obtener el lock, si no se puede, no se bloquea y se retorna -1, lo uso para liberar el lock
 *
 * SETLKW:
 *  * Intento obtener el lock de lectura, si no se puede, se bloquea hasta que se libere el lock
 *
**/

LockFile :: LockFile ( char* nombre ) {
    strcpy ( this->nombre,nombre );
    this->fl.l_type = F_WRLCK;
    this->fl.l_whence = SEEK_SET;
    this->fl.l_start = 0;
    this->fl.l_len = 0;
    this->fl.l_pid = getpid ();
    this->fd = open ( this->nombre,O_CREAT|O_WRONLY,0777 );
}

int LockFile :: tomarLock () {
    this->fl.l_type = F_WRLCK;
    int resultado = fcntl ( this->fd,F_SETLKW,&(this->fl) );
    return resultado;
}


int LockFile :: liberarLock () {
    this->fl.l_type = F_UNLCK;
    int resultado = fcntl ( this->fd,F_SETLK,&(this->fl) );
    return resultado;
}

int LockFile :: escribir ( char* buffer,int buffsize ) {
    lseek ( this->fd,0,SEEK_END );
    int resultado = write ( this->fd,buffer,buffsize );
    return resultado;
}

LockFile :: ~LockFile () {
    close ( this->fd );
}
