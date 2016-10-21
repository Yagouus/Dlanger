#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "sistEntrada.h"
#include "anaLexico.h"

//Variables
char c;
int e;
compLex* comp;

compLex* sigCompLex() {

    //Inicializamos valores
    comp = (compLex *) malloc(sizeof (compLex));
    comp->string = (char *) malloc(64);
    e = 1;
    c = sigCaracter();

    while (c) {

        //printf("%c", c);

        switch (e) {

                //Primer caracter que leemos
            case 1:
                if (isalnum(c) || c == '_') { //Cadenas alfanumericas
                    e = 2;
                } else if (isdigit(c)) { //Numeros
                    e = 3;
                } else if (c == ' ' || c == '\n') {
                    c = sigCaracter();
                } else if (c == '.' || c == ';' || c == '(' || c == ')' || c == '[' || c == ']' || c == ',' || c == '<' || c == '>' || c == '*' || c == '-' || c == '{' || c == '}') {
                    comp->string[strlen(comp->string)] = c;
                    c = sigCaracter();
                    e = 0;
                } else if (c == '/') {
                    e = 4;
                } else if (c == '=') {
                    e = 5;
                } else if (c == '+') {
                    comp->string[strlen(comp->string)] = c;
                    c = sigCaracter();
                    e = 6;
                } else if (c == '"') {
                    e = 7;
                } else {
                    return comp;
                }
                break;

                //Cadenas alfanumericas
            case 2:
                alfanum();
                break;

                //Numeros
            case 3:
                //numeros();
                return comp;
                break;

                //Comentarios
            case 4:
                comentarios();
                break;

                // == 
            case 5:
                if (c == '=') {
                    comp->string[strlen(comp->string)] = c;
                    c = sigCaracter();
                } else {
                    e = 0;
                }
                break;

                // +=
            case 6:
                if (c == '=') {
                    comp->string[strlen(comp->string)] = c;
                    c = sigCaracter();
                } else {
                    e = 0;
                }
                break;

                //Cadenas entre " "
            case 7:
                comillas();
                break;

                //Estado de aceptacion
            case 0:
                retroceder(); //"Devolvemos" un caracter           
                return comp; //Devolvemos el componente al anaSintactico
                break;

        }

        //c=sigCaracter();


    }


}

void alfanum() {
    if (isalnum(c)) {
        //printf("%c", c);
        comp->string[strlen(comp->string)] = c;
        c = sigCaracter();
    } else if (c == '_') { //Pueden contener _
        comp->string[strlen(comp->string)] = c;
        c = sigCaracter();
    } else {
        e = 0;
    }
} //Funcion para cadenas alfanumericas

void comentarios() {

    char p = c;
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

            if (c == t) { //Si encontramos un * o + comprobamos si se esta cerrando el comentario

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
        comp->string[strlen(comp->string)] = p;
        e = 0;
    }
} //Funcion para reconocer comentarios

void comillas() { //Funcion para cadenas entre comillas

    comp->string[strlen(comp->string)] = c;
    c = sigCaracter();

    while (c != '"') {
        if (c == '\\') {
            comp->string[strlen(comp->string)] = c;
            c = sigCaracter();
            comp->string[strlen(comp->string)] = c;
            c = sigCaracter();
        }
        comp->string[strlen(comp->string)] = c;
        c = sigCaracter();
    }


    comp->string[strlen(comp->string)] = c;
    c = sigCaracter();
    e = 0;
}