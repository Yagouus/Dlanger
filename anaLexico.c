#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "anaLexico.h"

//Variables
char c;
int e;
compLex* comp;
char * aux;

compLex* sigCompLex() {

    //Inicializamos valores
    aux = (char*) malloc(8);

    //Componente lexico
    comp = (compLex *) malloc(sizeof (compLex));

    //Estado
    e = 1;

    //Pedimos el siguiente caracter
    c = sigCaracter();

    while (c) {
        switch (e) {

                //Estado inicial
            case 1:
                einicial();
                break;

                //Cadenas alfanumericas
            case 2:
                alfanum();
                break;

                //Numeros
            case 3:
                numeros();
                break;

                //Comentarios
            case 4:
                comentarios();
                break;

                // == 
            case 5:
                igualigual();
                break;

                // += o ==
            case 6:
                masigualmasmas();
                break;

                //Cadenas entre " "
            case 7:
                comillas();
                break;

                //Estado de aceptacion
            case 0:
                retroceder(); //"Devolvemos" un caracter        
                comp->string = obtenerLexema();
                igualar();
                return comp; //Devolvemos el componente al anaSintactico
                break;

        }
    }


}

////AUTOMATA////

void einicial() { //Estado inicial

    //Cadenas alfanumericas
    if (isalpha(c) || c == '_') {
        e = 2;

        //Numeros
    } else if (isdigit(c)) {
        e = 3;

        //Comentarios
    } else if (c == '/') {
        e = 4;

        //Expresiones con =
    } else if (c == '=') {
        e = 5;

        //Expresiones con +
    } else if (c == '+') {
        e = 6;
        //Cadenas entre comillas
    } else if (c == '"') {
        e = 7;

        //Caracteres simples
    } else if (c == ';' || c == '(' || c == ')' || c == '[' || c == ']' || c == ',' || c == '<' || c == '>' || c == '*' || c == '-' || c == '{' || c == '}' || c == '.') {
        comp->id = (int) c; //Asignamos el codigo ascii
        c = sigCaracter();
        e = 0;

        // ' ' y '\n' se ignoran
    } else if (c == ' ' || c == '\n' || c == '\t') {
        igualar();
        c = sigCaracter();

        //Estado de aceptacion
    } else {
        error(CARACTER);
    }
}

void alfanum() { //Funcion para cadenas alfanumericas

    //Puede contener caracteres alfanumericos o _
    if (isalnum(c) || c == '_') {
        c = sigCaracter();

    } else {
        //Tratamos de registrar la cadena en la TS
        registrarTabla();
        e = 0;
    }
}

void comentarios() { //Funcion para reconocer comentarios

    //Añadimos /
    c = sigCaracter();

    //Comentarios de una linea
    if (c == '/') {

        //Añadimos hasta encontrar salto de linea
        while (c != '\n') {
            c = sigCaracter();
        }

        c = sigCaracter();
        igualar();
        e = 1;

        //Comentarios de bloque * o +
    } else if (c == '*' || c == '+') {
        int x = 1, l = 0; //Flag y nivel de anidamiento
        char t = c; //Tipo de comentario
        while (x != 0) {

            c = sigCaracter();

            if (c == t) { //Si encontramos un * o + comprobaremos si se esta cerrando el comentario

                c = sigCaracter();

                if (c == '/') { //Si se cierra comprobamos que sea el nivel de anidamiento 0
                    if (l == 0) {
                        e = 1;
                        igualar();
                        c = sigCaracter();
                        x = 0;
                    } else { //Si no es el 0 reducimos un nivel
                        l--;
                    }
                }

                //Si encontramos /+ se anida un comentario
            } else if (c == '/') {
                c = sigCaracter();
                if (c == '+') {
                    l++; //Aumentamos nivel de anidamiento
                }
            }
        }

        //Si solo es una / asignamos el tipo de lexema
    } else {
        comp->id = (int) '/';
        e = 0;
    }
}

void comillas() { //Funcion para cadenas entre comillas

    //Añadimos las " de apertura
    c = sigCaracter();

    //Hasta que encontremos las " de cierre
    while (c != '"') {

        //Si encontramos una barra de escape
        //Añadimos la barra y el caracter siguiente indistintamente
        if (c == '\\') {
            c = sigCaracter();
            c = sigCaracter();
        }

        //Añadimos el caracter que sea
        c = sigCaracter();
    }

    //Añadimos " de cierre
    c = sigCaracter();

    //Asignamos tipo de lexema
    comp->id = T_CADENA;

    //Aceptamos   
    e = 0;
}

void numeros() { //Numeros enteros y reales

    int f = 0, x = 1, k = 0;

    if (isdigit(c)) {

        //Si el primer caracter es un 0 comprobarmos si es binario
        if (c == '0') {
            binarios();
        }

        while (x != 0) {

            //Enteros
            if (isdigit(c) || c == '_') {
                enteros();

                //Reales
            } else if (c == '.') {
                reales();

                //Notacion cientifica
            } else if (c == 'e') {
                nCientifica();

            } else {
                x = 0;
            }

        }

        e = 0;

    }

}

void enteros() { //Reconoce numeros enteros sin signo

    //Pueden ser numeros o _ 
    while (isdigit(c) || c == '_') {
        c = sigCaracter();
    }

    //Asignamos el tipo de lexema
    comp->id = T_INTEGER;

}

void reales() { //Reconoce numeros de punto flotante

    //Añadimos el .
    c = sigCaracter();

    //Añadimos los numeros que siguen al punto
    enteros();

    //Asignamos el tipo de lexema
    comp->id = T_FLOAT;
}

void binarios() {

    //Añadimos el 0
    c = sigCaracter();

    //Comprobamos que la siguiente letra sea b 0 B
    if (c == 'b' || c == 'B') {

        //Añadimos la b
        c = sigCaracter();

        //Cadena mal formada
        if (c != 1 || c != 0) {
            error(LEXEMA);
            return;
        }

        //Añadimos los numeros que siguen
        while (c == '1' || c == '0') {
            c = sigCaracter();

        }

    } else {
        comp->id = T_INTEGER;
        return;
    }

    //Asignamos el tipo de lexema
    comp->id = T_BINARIO;
}

void nCientifica() {

    //Añadimos la e
    c = sigCaracter();

    //Despues de la e tiene que haber un + o un -
    if (c == '+' || c == '-' || isdigit(c)) {
        c = sigCaracter();

        //Añadimos el resto de numeros
        enteros();

        //Aceptamos
        comp->id = T_NCIENTIFICA;

        //Error, numero mal formado
    } else {
        error(LEXEMA);        
    }
}

void masigualmasmas() { //Reconoce las cadenas += y ++

    //Añadimos el +
    c = sigCaracter();

    //Masigual
    if (c == '=') {
        comp->id = MASIGUAL;
        c = sigCaracter();
        e = 0;

        //Masmas
    } else if (c == '+') {
        comp->id = MASMAS;
        c = sigCaracter();
        e = 0;

        //Es solo un +
    } else {
        comp->id = (int) '+';
        e = 0;
    }
}

void igualigual() { //Comprobamos ==

    //Añadimos =
    c = sigCaracter();

    //Si el siguiente caracter es otro =
    if (c == '=') {
        comp->id = IGUALIGUAL;
        c = sigCaracter();
        e = 0;

        //Si solo es 1 un =
    } else {
        comp->id = (int) '=';
        e = 0;
    }

}

////OTRAS FUNCIONES////

void registrarTabla() { //Funcion que registra un ID en la tabla de simbolos

    //"Devolvemos" un caracter   
    retroceder();

    //Obtenemos el lexema
    comp->string = obtenerLexema();

    //Pasamos al caracter siguiente
    sigCaracter();

    //Insertamos el componente en la tabla
    insertaElemento(comp);
}
