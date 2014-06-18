#ifndef MENSAJES_H_
#define MENSAJES_H_

#define	PETICION	1
#define	RESPUESTA	2
#define TEXTO_SIZE	255
#include <sys/wait.h>


typedef struct mensaje {
	long mtype;
	int id;
    pid_t pid;
    float capacidad;
    unsigned int empleadoID;
} mensaje;

#endif /* MENSAJES_H_ */
