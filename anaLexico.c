#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "definiciones.h"
#include "sistEntrada.h"
#include "anaLexico.h"

//Variables
char c;
int e;
compLex* comp;

compLex* sigCompLex() {

    //Inicializamos valores
    comp = (compLex *) malloc(sizeof (compLex));
    comp->string = (char *) malloc(64); //OJO CAMBIAR TAMAÑO DEL MALLOC
    e = 1;
    c = sigCaracter();

    int i = 0;

    while (c) {
        switch (e) {

                //Primer caracter que leemos
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

                // +=
            case 6:
                masigualmasmas();
                break;

                //Cadenas entre " "
            case 7:
                comillas();
                break;

            case 8:
                binarios();
                break;

                //Estado de aceptacion
            case 0:
                retroceder(); //"Devolvemos" un caracter           
                return comp; //Devolvemos el componente al anaSintactico
                e = 1;
                //free(comp->string); //Liberamos memoria
                //free(comp);
                break;

        }
        i++;
    }


}

void alfanum() { //Funcion para cadenas alfanumericas palabras reservadas o IDs

    if (isalnum(c) || c == '_') {
        comp->string[strlen(comp->string)] = c;
        c = sigCaracter();
    } else {
        registrarTabla();
        e = 0;
    }
}

void comentarios() { //Funcion para reconocer comentarios

    //Añadimos /
    comp->string[strlen(comp->string)] = c;
    c = sigCaracter();

    //Comentarios de una linea
    if (c == '/') {
        while (c != '\n') {
            c = sigCaracter();
        }
        c = sigCaracter();
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

    } else {
        comp->id = (int) '/';
        e = 0;
    }
}

void comillas() { //Funcion para cadenas entre comillas

    //Añadimos las " de apertura
    comp->string[strlen(comp->string)] = c;
    c = sigCaracter();

    //Hasta que encontremos las " de cierre
    while (c != '"') {
        
        //Si encontramos una barra de escape
        //Añadimos la barra y el caracter siguiente indistintamente
        if (c == '\\') {
            comp->string[strlen(comp->string)] = c;
            c = sigCaracter();
            comp->string[strlen(comp->string)] = c;
            c = sigCaracter();
        }
        comp->string[strlen(comp->string)] = c;
        c = sigCaracter();
    }

    //Añadimos " de cierre
    comp->string[strlen(comp->string)] = c;
    
    //Asignamos tipo de lexema
    comp->id = T_CADENA;
    
    //Aceptamos
    c = sigCaracter();
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

void enteros() {

    while (isdigit(c) || c == '_') {
        comp->string[strlen(comp->string)] = c;
        c = sigCaracter();
    }

    comp->id = T_INTEGER;

}

void reales() {

    int f = 0, x = 1, k = 0;

    //Añadimos el .
    comp->string[strlen(comp->string)] = c;
    c = sigCaracter();

    //Añadimos los numeros que siguen al punto
    enteros();

    comp->id = T_FLOAT;
}

void binarios() {

    //Añadimos el 0
    comp->string[strlen(comp->string)] = c;
    c = sigCaracter();

    //Comprobamos que la siguiente letra sea b 0 B
    if (c == 'b' || c == 'B') {

        //Añadimos la b
        comp->string[strlen(comp->string)] = c;
        c = sigCaracter();

        //Añadimos todos los numeros
        while (c == '1' || c == '0') {
            comp->string[strlen(comp->string)] = c;
            c = sigCaracter();
        }

        //Si solo es un 0 lo devovemos
    } else {
        e = 0;
    }

    //Aceptamos
    comp->id = T_INTEGER;
    e = 0;
}

void nCientifica() {

    //Añadimos la e
    comp->string[strlen(comp->string)] = c;
    c = sigCaracter();

    //Despues de la e tiene que haber un + o un -
    if (c == '+' || c == '-') {
        comp->string[strlen(comp->string)] = c;
        c = sigCaracter();

        //Añadimos el resto de numeros
        enteros();

        //Aceptamos
        comp->id = T_FLOAT;
        e = 0;

        //Error, numero mal formado
    } else {

    }
}

void masigualmasmas() { //Reconoce las cadenas += y ++
    comp->string[strlen(comp->string)] = c;
    c = sigCaracter();

    //Masigual
    if (c == '=') {
        comp->string[strlen(comp->string)] = c;
        comp->id = MASIGUAL;
        c = sigCaracter();
        e = 0;

        //Masmas
    } else if (c == '+') {
        comp->string[strlen(comp->string)] = c;
        comp->id = MASMAS;
        c = sigCaracter();
        e = 0;

    } else {
        e = 0;
    }
}

void igualigual() { //Comprobamos ==

    //Añadimos =
    comp->string[strlen(comp->string)] = c;
    c = sigCaracter();

    //Si el siguiente caracter es otro =
    if (c == '=') {
        comp->string[strlen(comp->string)] = c;
        comp->id = IGUALIGUAL;
        c = sigCaracter();
        e = 0;

        //Si solo es 1 un =
    } else {
        comp->id = (int) '=';
        e = 0;
    }

}

void einicial() { //Funcion que redirige a los demas automatas
    if (isalpha(c) || c == '_') { //Cadenas alfanumericas
        e = 2;
    } else if (isdigit(c)) { //Numeros
        e = 3;
    }/* else if (c == '0') {
        e = 8;
    }else if (c == '.') {
        comp->string[strlen(comp->string)] = c;
        c = sigCaracter();
        e = 3;
    }*/
        //El espacio y salto de linea los ignoramos
    else if (c == ' ' || c == '\n') {
        c = sigCaracter();

        //Caracteres simples
    } else if (c == ';' || c == '(' || c == ')' || c == '[' || c == ']' || c == ',' || c == '<' || c == '>' || c == '*' || c == '-' || c == '{' || c == '}' || c == '.') {
        comp->string[strlen(comp->string)] = c; //Asignamos el lexema
        comp->id = (int) c; //Asignamos el codigo ascii
        c = sigCaracter();
        e = 0;

        //Posibles comentarios
    } else if (c == '/') {
        e = 4;

        //Expresiones con igual
    } else if (c == '=') {
        e = 5;

        //Expresiones con +
    } else if (c == '+') {
        e = 6;

        //Posibles cadenas entre comillas
    } else if (c == '"') {
        e = 7;
    } else {
        e = 0;
    }
}

void registrarTabla() { //Funcion que registra un ID en la tabla de simbolos
    insertaElemento(comp);
}