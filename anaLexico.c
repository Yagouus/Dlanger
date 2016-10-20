#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "sistEntrada.h"
#include "anaLexico.h"

compLex* sigCompLex() {

    int f = 1;

    compLex* comp;
    comp = (compLex *) malloc(sizeof (compLex));
    comp->string = (char *) malloc(16);

    int e = 1;

    char c;
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
                } else if (c == '.' || c == ';' || c == '(' || c == ')' || c == '[' || c == ']' || c == ',' || c == '=' || c == '<' || c == '>' || c == '*' || c == '-') {
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
                c = sigCaracter();
                if (c == '/') {
                    while(c != '\n'){
                        c = sigCaracter();
                    }
                    c = sigCaracter();
                    e = 1;
                }else{
                    e = 0;
                }
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