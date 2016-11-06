#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistEntrada.h"


//DEFINICION DE VARIABLES
FILE * f; //Puntero al fichero a compilar
char * buffer1; //Memoria 1
char * buffer2; //Memoria 2
char * inicio; //Puntero al ppio de un lexema
char * final; //Puntero al final de un lexema
int mem = 0; //Indica la memoria que se esta leyendo
int flag = 0; //Indica si las dos memorias estan cargadas

void load() { //Funcion de inicializacion

    buffer1 = (char *) malloc(TAM + 1 * sizeof (char *));
    buffer2 = (char *) malloc(TAM + 1 * sizeof (char *));

    //Tratamos de abrir el fichero
    if ((f = fopen("regression.d", "r")) == NULL) {
        error(FICHERO);
        return;
    }

    //Cargamos el fichero en memoria
    cargaMemorias();

    //Apuntamos al comienzo del fichero
    inicio = final;

}

char sigCaracter() { //"Devuelve" el caracter siguiente

    if (*final == EOF) {

        //Comprobamos si es el final de fichero
        if (finalFichero) {
            return EOF;
        }


        if (flag == 0) {
            cargaMemorias();
        } else {
            cambiaMemorias();
            flag = 0;
        }
    }

    return *(final++);

}

void igualar() { //Iguala el puntero inicial al final
    inicio = final;
}

char* obtenerLexema() { //Devuelve un lexema

    //Puntero auxiliar al caracter actual
    char* aux = inicio;

    //Lexema que se devolverá
    char* lexema = (char*) malloc(TAM);

    //Mientras no se llegue al final del lexema
    while (aux != final) {

        if (*aux == EOF) {
            if (mem == 0) {
                aux = buffer2;
            } else {
                aux = buffer1;
            }
        }

        //Anadimos el caracter actual
        lexema[strlen(lexema)] = *aux;

        //Aumentamos posicion
        aux++;
    }

    return lexema;
}

void retroceder() { //Retrocede el puntero a final
    if (mem == 0 && final == buffer1) {
        final = buffer2 + TAM - 1;
        flag = 1;
    } else if (mem == 1 && final == buffer2) {
        final = buffer2 + TAM - 1;
        flag = 1;
    } else {
        final--;
    }
}

void close() { //Funcion de liberacion de memoria

    //Cerramos el fichero
    fclose(f);

    //Liberamos el buffer1
    free(buffer1);

    //Liberamos el buffer2
    free(buffer2);
}

void cargaBuffer1() {
    int c = fread(buffer1, 1, TAM, f);
    fread(buffer1, TAM, 1, f);
    //strcat(buffer1, "\0");
    buffer1[c] = EOF;
    final = buffer1;
    mem = 1;
}

void cargaBuffer2() {
    int c = fread(buffer2, 1, TAM, f);
    //strcat(buffer2, "\0");
    buffer2[c] = EOF;
    final = buffer2;
    char a = *final;
    mem = 0;
}

void cargaMemorias() {
    if (mem == 0) {
        cargaBuffer1();
    } else {
        cargaBuffer2();
    }
}

void cambiaMemorias() {
    if (mem == 0) {
        mem = 1;
    } else {
        mem = 0;
    }
}

int finalFichero() {
    if (mem == 0) {
        int x = final - buffer2;
        if (final - buffer2 != TAM) {
            return 1;
        }
    } else {
        int x = final - buffer1;
        if (final - buffer1 != TAM) {
            return 1;
        }
    }
    return 0;
}