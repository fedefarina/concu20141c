#include "Semaforo.h"

/**
 * @brief Semaforo::Semaforo
 * @param nombre Se crea una clave a partir de esto
 * @param valorInicial para el semaforo
 *
 * Semaforo disponible -> Contador >0
 * Semaforo ocupado    -> Semaforo <=0
 *
 */

Semaforo::Semaforo(){

}

Semaforo :: Semaforo ( char* nombre,int valorInicial ) {

    key_t clave = ftok ( nombre,'a' );
    //clave a partir de ftok,cantidad de semaforos, creo el semaforo si no existe
    this->id = semget ( clave,1,0666 | IPC_CREAT );
    this->inicializar (valorInicial);
}


Semaforo :: Semaforo ( char* nombre) {
    key_t clave = ftok ( nombre,'a' );
    this->id = semget ( clave,1,0666 | IPC_CREAT );
}

int Semaforo :: inicializar (int valorInicial) {

    union semnum {
        int val;
        struct semid_ds* buf;
        ushort* array;
    };

    semnum init;
    init.val = valorInicial;

    //idSemaforo, numeroSemaforo dentro del conjunto, operacion, valor inicial
    int resultado = semctl ( this->id,0,SETVAL,init );
    return resultado;
}

/**
 * @brief Semaforo::p Sumo uno al contador, esperar(wait)
 * @return valor del contador
 */
int Semaforo :: p () {

    struct sembuf operacion;

    operacion.sem_num = 0;	// numero de semaforo
    operacion.sem_op  = -1;	// restar 1 al semaforo
    operacion.sem_flg = SEM_UNDO;

    int resultado = semop ( this->id,&operacion,1 );
    return resultado;
}

/**
 * @brief Semaforo::v Resto uno al contador, signal
 * @return valor del contador
 */
int Semaforo :: v () {

    struct sembuf operacion;

    operacion.sem_num = 0;	// numero de semaforo
    operacion.sem_op  = 1;	// sumar 1 al semaforo
    operacion.sem_flg = SEM_UNDO;

    int resultado = semop ( this->id,&operacion,1 );
    return resultado;
}

Semaforo::~Semaforo() {
}


void Semaforo :: eliminar () {
    semctl ( this->id,0,IPC_RMID );
}
