#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistEntrada.h"


//DEFINICION DE VARIABLES
FILE * f; //Puntero al fichero a compilar
char * buffer; //Buffer en el que se guarda el fichero
char * puntero; //Puntero al caracter actual
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
    if (fread(buffer, TAM, 1, f) != 1) {
        error("Error cargando el archivo en memoria");
    }
    
    strcat(buffer, "\0");
    
    //Apuntamos al comienzo del fichero
    puntero = buffer;
    inicio = buffer;
    final = buffer;

}

char sigCaracter() { //Devuelve caracter a caracter
    
    while(*final != EOF){
        return *(final++);        
    }
    
}

void igualar(){ //Iguala el puntero inicial al final
    inicio = final;
}

char* obtenerLexema(){ //Devuelve un componente lexico
    char* lexema = (char*) malloc(64);
    while(inicio != final){
        lexema[strlen(lexema)] = *inicio;
        inicio++;
    }
    
    //printf("%s", lexema);
    return lexema;
}

void close() { //Funcion de liberacion de memoria
    fclose(f);
    free(buffer);
}

void retroceder(){
    final--;
}

