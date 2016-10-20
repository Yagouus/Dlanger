#include <stdio.h>
#include <stdlib.h>
#include "sistEntrada.h"


//DEFINICION DE VARIABLES
FILE * f; //Puntero al fichero a compilar
char * buffer; //Buffer en el que se guarda el fichero
char * puntero; //Puntero al caracter actual


void Load() { //Funcion de inicializacion

    buffer = (char *) malloc(TAM * sizeof (char *));

    //Tratamos de abrir el fichero
    if ((f = fopen("regression.d", "r")) == NULL) {
        error("Error abriendo archivo");
        return;
    }

    //Cargamos el fichero en memoria
    if (fread(buffer, TAM, 1, f) != 1) {
        error("Error cargando el archivo en memoria");
    }
    
    //Apuntamos al comienzo del fichero
    puntero = buffer;

}

char sigCaracter() { //Devuelve caracter a caracter
    
    while(*puntero != EOF){
        return *(puntero++);        
    }
    
    return NULL;
}

void Close() { //Funcion de liberacion de memoria
    fclose(f);
    free(buffer);
}

void retroceder(){
    puntero--;
}

