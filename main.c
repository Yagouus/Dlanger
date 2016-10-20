#include <stdio.h>
#include <stdlib.h>
#include "sistEntrada.h"
#include "anaSintactico.h"
#include "anaLexico.h"

int main(int argc, char** argv) {
    
    //Inicializamos tabla de simbolos
    
    //Cargamos el fichero
    Load();
    
    //Lanzamos en analizador sintactico
    analisisSintactico();
    
    //Liberamos memoria
    Close();
    


    return (EXIT_SUCCESS);
}

