#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tablaSimbolos.h"

arbol tabla;

////FUNCIONES DE UN ARBOL////

void crea(arbol *A) { //Crea un arbol
    *A = NULL;
}

void destruye(arbol *A) { //Destruye un arbol
    if (*A != NULL) {
        destruye(&(*A)->izq);
        destruye(&(*A)->der);
        free(*A);
        *A = NULL;
    }
}

unsigned esVacio(arbol A) { //Comprueba si el arbol esta vacio
    return A == NULL;
}

void inserta(arbol* A, compLex* comp) { //Inserta un elemento en el arbol
    if (esVacio(*A)) {
        *A = (arbol) malloc(sizeof (struct nodo));
        (*A)->lexema = comp;
        (*A)->izq = NULL;
        (*A)->der = NULL;
    } else if (strcmp(comp->string, (*A)->lexema->string) < 0) {
        inserta(&(*A)->izq, comp);
    } else
        inserta(&(*A)->der, comp);
}

compLex* busca(arbol A, compLex *comp) { //Busca un elemento en el arbol
    if (esVacio(A))
        return NULL;
    else if (strcmp(comp->string, A->lexema->string) == 0)
        return A->lexema;
    else if (strcmp(comp->string, A->lexema->string) < 0)
        return busca(izq(A), comp);
    else
        busca(der(A), comp);
}

arbol izq(arbol A) { //Devuelve el nodo izq
    return A->izq;
}

arbol der(arbol A) { //Devuelve el nodo dcho
    return A->der;
}

void imprimeElemento(arbol* A) { //Imprime un elemento del arbol

    //Se recorre el arbol y se imprimen todos los elementos
    if (*A != NULL) {
        imprimeElemento(&(*A)->izq);
        printf("<%d><%s>\n", (*A)->lexema->id, (*A)->lexema->string);
        imprimeElemento(&(*A)->der);
        
    }

}


////FUNCIONES TABLA DE SIMBOLOS////

void crearTabla() { //Crea la tabla de simbolos
    crea(&tabla);
}

void destruyeTabla() { //Destruye la tabla de simbolos
    destruye(&tabla);
}

void inicializa() { //Inicializa la tabla de simbolos

    //Funciones
    insertarPalReservada("IMPORT", IMPORT);
    insertarPalReservada("WHILE", WHILE);
    insertarPalReservada("FOREACH", FOREACH);
    insertarPalReservada("RETURN", RETURN);
    insertarPalReservada("CAST", CAST);

    //Tipos de dato
    insertarPalReservada("INT", INT);
    insertarPalReservada("DOUBLE", DOUBLE);
    insertarPalReservada("VOID", VOID);

}

int buscarEnTabla(compLex* comp) { //Busca un componente en la tabla de simbolos
    if (busca(tabla, comp) != NULL) {
        return 1;
    };
}

void insertarEnTabla(compLex* comp) { //Inserta un componente en la tabla de simbolos
    inserta(&tabla, comp);
}

void insertarPalReservada(char* lexema, int id) { //Inserta un componente léxico a partir de un lexema y un identificador

    //Definimos el componente lexico
    compLex* comp;
    comp = (compLex *) malloc(sizeof (compLex));
    comp->string = (char *) malloc(16);
    comp->id = id;

    //Pasamos el lexema a minusculas
    for (int i = 0; i < sizeof (lexema); i++) {
        comp->string[i] = tolower(lexema[i]);
    }

    //Insertamos el componente en la tabla
    insertarEnTabla(comp);
}

void imprimeTabla() { //Imprime el contenido de la tabla
    printf("\n/////TABLA DE SIMBOLOS/////\n");
    imprimeElemento(&tabla);
    printf("////////////////////////////\n\n");
}

void asignarID(compLex* comp) { //Busca un lexema en la tabla y le asigna el id correspondiente
    compLex* aux = busca(tabla, comp);
    comp->id = aux->id;

}

void insertaElemento(compLex* comp) { //Inserta un componente lexico en la tabla

    //Si el componente no esta en la tabla es un identificador
    if (buscarEnTabla(comp) != 1) {
        comp->id = ID; //Se le asigna como tipo ID
        insertarEnTabla(comp); //Se inserta en la tabla
        
        //Si esta en la tabla se le asigna el tipo que le corresponda
    } else {
        asignarID(comp);
    }
}
