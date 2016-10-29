#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tablaSimbolos.h"
#include "definiciones.h"

arbol tabla;

void crea() {
    tabla = NULL;
}

void destruye() {
    destruye(tabla->izq);
    destruye(tabla->der);
    free(tabla);
    tabla = NULL;
}

unsigned esVacio(arbol A) {
    return A == NULL;
}

void inserta(arbol* A, compLex* comp) {
    if (esVacio(*A)) {
        *A = (arbol) malloc(sizeof (struct nodo));
        (*A)->lexema = comp;
        (*A)->izq = NULL;
        (*A)->der = NULL;
    } else if (strcmp(comp->string, (*A)->lexema->string) < 0) {
        inserta(&(*A)->izq, comp);
    } else
        inserta(&(*A)->der, comp);
    
    //printf("%s: %d \n", (*A)->lexema->string, (*A)->lexema->id);
}

compLex* busca(arbol A, compLex *comp) {
    if (esVacio(A))
        printf("Clave inexistente\n");
    else if (strcmp(comp->string, A->lexema->string) == 0)
        return A->lexema;
    else if (strcmp(comp->string, A->lexema->string) < 0)
        return busca(izq(A), comp);
    else
        busca(der(A), comp);
}

arbol izq(arbol A) {
    return A->izq;
}

arbol der(arbol A) {
    return A->der;
}

void imprimeElemento(arbol* A){
    if (*A != NULL) {
        imprimeElemento(&(*A)->izq);
        imprimeElemento(&(*A)->der);
        printf("<%d><%s>\n", (*A)->lexema->id, (*A)->lexema->string);
    }

}

void inicializa() {
    insertarPalReservada("IMPORT", 300);
    insertarPalReservada("WHILE", 301);
}

int buscarEnTabla(compLex* comp) {
    if (busca(tabla, comp) == NULL) {
        return NULL;
    } else {
        return 1;
    };
}

int insertarEnTabla(compLex* comp) {
    inserta(&tabla, comp);
}

void insertarPalReservada(char* lexema, int id) {

    //Definimos el componente lexico
    compLex* comp;
    comp = (compLex *) malloc(sizeof (compLex));
    comp->string = (char *) malloc(16);
    comp->id = id;

    //Pasamos el lexema a minusculas
    for (int i = 0; i < sizeof (lexema); i++) {
        comp->string[i] = tolower(lexema[i]);
    }

    insertarEnTabla(comp);
}

void imprime(){
    printf("\n/////TABLA DE SIMBOLOS/////\n");
    imprimeElemento(&tabla);
    printf("///////////////////////////\n\n");
}

