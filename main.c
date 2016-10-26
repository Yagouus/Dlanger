#include <stdio.h>
#include <stdlib.h>
#include "sistEntrada.h"
#include "anaLexico.h"
#include "anaSintactico.h"


int main(int argc, char** argv) {
    
    //Creamos e inicializamos tabla de simbolos
    crea();
    inicializa();
    
    //Cargamos el fichero
    Load();
    
    //Lanzamos en analizador sintactico
    analisisSintactico();
    
    //Liberamos memoria
    Close();
    


    return (EXIT_SUCCESS);
}

