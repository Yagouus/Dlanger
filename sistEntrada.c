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
int cargadas = 0; //Indica si las dos memorias estan cargadas

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
        if (finalFichero() == 1) {
            return EOF;
        }

        //Comprobamos si las dos memorias estan cargadas
        if (cargadas == 0) {
            
            //Cargamos la que no lo este
            cargaMemorias();
            
        } else {
            
            //Cambiamos la memoria a usar
            cambiaMemorias();            
            
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
    char* fin = final;

    //Lexema que se devolverá
    char* lexema = (char*) malloc(TAM);

    //Mientras no se llegue al final del lexema
    while (aux != final) {

        if (*aux == EOF) {
            if (mem == 0) {
                aux = buffer2;
                continue;
            } else {
                aux = buffer1;
                continue;
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

    //Si el lexema empieza en un buffer
    //Pero termina en el primer caracter del siguiente
    if (mem == 1 && final == buffer1) {
        final = buffer2 + TAM - 1;
        cargadas = 1;
    } else if (mem == 0 && final == buffer2) {
        final = buffer2 + TAM - 1;
        cargadas = 1;
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

void cargaBuffer1() { //Carga el fichero en la memoria 1

    //Anadimos EOF al final de los caracteres leidos
    buffer1[fread(buffer1, 1, TAM, f)] = EOF;

    //Apuntamos final al nuevo buffer cargado
    final = buffer1;

    //Cambiamos el indicador de memoria usada
    mem = 1;
}

void cargaBuffer2() { //Carga el fichero en la memoria 2

    //Anadimos EOF al final de los caracteres leidos
    buffer2[fread(buffer2, 1, TAM, f)] = EOF;

    //Apuntamos final al nuevo buffer cargado
    final = buffer2;
    char* fin = buffer2;

    //Cambiamos el indicador de memoria usada
    mem = 0;
}

void cargaMemorias() { //Cargamos la memoria que corresponda
    if (mem == 0) {
        cargaBuffer1();
    } else {
        cargaBuffer2();
    }
}

void cambiaMemorias() { //"Cambiamos" la memoria que se esta usando
    
    if (mem == 0) {
        mem = 1;
    } else {
        mem = 0;
    }
    
    cargadas = 0;
}

int finalFichero() {

    //Segun la memoria que se este usando
    //Comprobamos si el EOF es el de final de buffer
    //O el de final de cadena
    if (mem == 0) {
        if (final - buffer2 != TAM) {
            return EOF;
        }
    } else {
        if (final - buffer1 != TAM) {
            return EOF;
        }
    }
}