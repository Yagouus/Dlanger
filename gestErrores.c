#include <stdio.h>
#include <stdlib.h>
#include "gestErrores.h"

//Con el tipo de error generamos el mensaje

void error(int error) {
    switch (error) {
        case 1:
            mostrar("Error ocurrido abriendo el fichero");
            exit(0);
            break;
        case 2:
            mostrar("Lexema mal formado");
            break;
        case 3:
            mostrar("Caracter ilegible");
            break;
    }
}

//Muestra el error ocurrido por pantalla

void mostrar(char* mensaje) {
    printf("ERROR -> %s\n", mensaje);
}