#include <stdio.h>
#include <stdlib.h>
#include "sistEntrada.h"
#include "anaLexico.h"
#include "anaSintactico.h"

int main(int argc, char** argv) {

    //Cargamos el fichero a compilar
    load(argv[1]);

    //Creamos, inicializamos y mostramos la TS
    crearTabla();
    inicializa();
    imprimeTabla();

    //Lanzamos en analizador sintactico
    analisisSintactico();

    //Volvemos a imprimir la TS
    imprimeTabla();

    //Liberamos memoria
    destruyeTabla();
    liberarLexico();
    close();
}

