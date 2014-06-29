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

Semaforo :: Semaforo (char* nombre, unsigned int valorInicial, unsigned int cantidad) {

    key_t clave = ftok ( nombre,'a' );
    //clave a partir de ftok,cantidad de semaforos, creo el semaforo si no existe
    this->id = semget ( clave,cantidad,0666 | IPC_CREAT );

    if (this->id < 0)
        Logger::debug(getpid(), "Error al crear semaforo\n");

    for (unsigned int i = 0; i < cantidad; i++)
        this->inicializar (valorInicial, i);
}

Semaforo :: Semaforo ( char* nombre,unsigned int cantidad) {
    key_t clave = ftok ( nombre,'a' );
    this->id = semget ( clave,cantidad,0666 | IPC_CREAT );

    if (this->id < 0)
        Logger::debug(getpid(), "Error al crear semaforo\n");
}

int Semaforo :: inicializar (int valorInicial, unsigned int posicion) {

    union semnum {
        int val;
        struct semid_ds* buf;
        ushort* array;
    };

    semnum init;
    init.val = valorInicial;

    //idSemaforo, numeroSemaforo dentro del conjunto, operacion, valor inicial
    int resultado = semctl ( this->id,posicion,SETVAL,init );
    return resultado;
}

int Semaforo::getValue(unsigned int posicion) const{
    return semctl (this->id,posicion,GETVAL,0);
}

/**
 * @brief Semaforo::p Sumo uno al contador, esperar(wait)
 * @return valor del contador
 */
int Semaforo :: p (unsigned int posicion) {

    struct sembuf operacion;

    operacion.sem_num = posicion;	// numero de semaforo
    operacion.sem_op  = -1;	// restar 1 al semaforo
    operacion.sem_flg = SEM_UNDO;

    int resultado = semop ( this->id,&operacion,1 );
    return resultado;
}

/**
 * @brief Semaforo::v Resto uno al contador, signal
 * @return valor del contador
 */
int Semaforo :: v (unsigned int posicion) {

    struct sembuf operacion;

    operacion.sem_num = posicion;	// numero de semaforo
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
