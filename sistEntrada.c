#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistEntrada.h"


//DEFINICION DE VARIABLES
FILE * f; //Puntero al fichero a compilar
char * buffer; //Buffer en el que se guarda el fichero
char * inicio; //Puntero al ppio de un lexema
char * final; //Puntero al final de un lexema

void load() { //Funcion de inicializacion

    buffer = (char *) malloc(TAM + 1 * sizeof (char *));

    //Tratamos de abrir el fichero
    if ((f = fopen("regression.d", "r")) == NULL) {
        error("Error abriendo archivo");
        return;
    }

    //Cargamos el fichero en memoria
    fread(buffer, TAM, 1, f);

    //Le añadimos EOF
    strcat(buffer, "\0");

    //Apuntamos al comienzo del fichero
    inicio = buffer;
    final = buffer;

}

char sigCaracter() { //"Devuelve" el caracter siguiente

    //Avanzamos el puntero al final del lexema
    while (*final != EOF) {
        return *(final++);
    }

}

void igualar() { //Iguala el puntero inicial al final
    inicio = final;
}

char* obtenerLexema() { //Devuelve un lexema

    //Puntero auxiliar al caracter actual
    char* aux = inicio;

    //Lexema que se devolverá
    char* lexema = (char*) malloc(final - inicio);

    while (aux != final) {
        
        //Anadimos el caracter actual
        lexema[strlen(lexema)] = *aux;
        
        //Aumentamos posicion
        aux++;
    }

    return lexema;
}

void close() { //Funcion de liberacion de memoria
    fclose(f);
    free(buffer);
}

void retroceder() {
    final--;
}

