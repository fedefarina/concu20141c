#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "MemoriaCompartida.h"

using namespace std;

int main () {

    pid_t procId = fork ();

    if ( procId == 0 ) {

        MemoriaCompartida<int> memoria;
        int estadoMemoria = memoria.crear ( (char*) "../tp1/main.cpp",'R' );

        cout << "Hijo: duermo 2 segundos...(espero que el padre haya escrito en la memoria)" << endl;
        sleep ( 2 );

        if ( estadoMemoria == SHM_OK ) {
            int resultado = memoria.leer ();
            cout << "Hijo: leo el numero " << resultado << " de la memoria compartida" << endl;
            memoria.liberar ();
        } else {
            cout << "Hijo: error en memoria compartida: " << estadoMemoria << endl;
        }
        cout << "Hijo: fin del proceso" << endl;
        exit ( 0 );

    } else {

        MemoriaCompartida<int> memoria;
        int estadoMemoria = memoria.crear ( (char*) "../tp1/main.cpp",'R' );
        if ( estadoMemoria == SHM_OK ) {

            // escribe un dato para el hijo
            int random = 5;
            cout << "Padre: escribo el numero " << random << " en la memoria compartida" << endl;
            memoria.escribir ( random );

            // espera a que termine el hijo
            int estado = 0;
            wait ( &estado );
            // libera la memoria
            memoria.liberar ();
        } else {
            cout << "Padre: error en memoria compartida: " << estadoMemoria << endl;
        }

        cout << "Padre: fin del proceso" << endl;
        exit ( 0 );
    }
}

