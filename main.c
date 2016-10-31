#include <stdio.h>
#include <stdlib.h>
#include "sistEntrada.h"
#include "anaLexico.h"
#include "anaSintactico.h"


int main(int argc, char** argv) {
    
    //Creamos, inicializamos y mostramos la TS
    crearTabla();
    inicializa();
    imprimeTabla();
    
    //Cargamos el fichero a compilar
    load();
    
    //Lanzamos en analizador sintactico
    analisisSintactico();
    
    //Volvemos a imprimir la TS
    imprimeTabla();
    
    //Liberamos memoria
    destruyeTabla();
    close();
    


    return (EXIT_SUCCESS);
}

