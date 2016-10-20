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
    comp->string = (char *) malloc(16);
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
                } else if (c == '.' || c == ';' || c == '(' || c == ')' || c == '[' || c == ']' || c == ',' || c == '=' || c == '<' || c == '>' || c == '*' || c == '-' || c == '{' || c == '}') {
                    comp->string[strlen(comp->string)] = c;
                    c = sigCaracter();
                    e = 0;
                } else if (c == '/') {
                    e = 4;
                } else {
                    return comp;
                }
                break;

                //Cadenas alfanumericas
            case 2:
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
                break;

                //Numeros
            case 3:
                //Naturales
                //Enteros
                //Binarios
                return comp;
                break;

                //Comentarios
            case 4:
                comentarios();
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

//Funcion para reconocer comentarios
void comentarios() {
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
        int x = 1, l = 0;
        char t = c;
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
        e = 0;
    }
}