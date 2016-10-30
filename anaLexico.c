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
                free(comp->string); //Liberamos memoria
                free(comp);
                break;

        }
        i++;
    }


}

void alfanum() { //Funcion para cadenas alfanumericas
    //Seran palabras reservadas o identificadores
    if (isalnum(c)) {
        comp->string[strlen(comp->string)] = c;
        c = sigCaracter();
    } else if (c == '_') { //Pueden contener _
        comp->string[strlen(comp->string)] = c;
        c = sigCaracter();
    } else {
        e = 0;
        registrarTabla(comp);
    }
}

void comentarios() { //Funcion para reconocer comentarios

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
        comp->string[strlen(comp->string)] = p;
        e = 0;
    }
}

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

void numeros() { //Numeros enteros y reales

    int f = 0, x = 1, k = 0;

    if (isdigit(c)) {

        while (x != 0) {

            //Enteros
            if (isdigit(c) || c == '_') {
                comp->string[strlen(comp->string)] = c;
                c = sigCaracter();

                //Reales
            } else if (c == '.') {
                if (f == 0) {
                    comp->string[strlen(comp->string)] = c;
                    c = sigCaracter();
                    f++;
                } else {
                    x = 0;
                }

                //Notacion cientifica
            } else if (c == 'e') {

                comp->string[strlen(comp->string)] = c;
                c = sigCaracter();

                //Despues de la e tiene que haber un + o un -
                if (c == '+' || c == '-') {
                    comp->string[strlen(comp->string)] = c;
                    c = sigCaracter();

                    //Tras el signo hay un numero escrito en notación normal, lo analizamos
                    numeros();

                } else {
                    x = 0;
                }

                x = 0;

            } else {
                x = 0;
            }
        }

        e = 0;

    } else {
        e = 0;
    }

}

void binarios() {

    //Añadimos el 0
    comp->string[strlen(comp->string)] = c;
    c = sigCaracter();

    //Comprobamos que sea binario
    if (c == 'b' || c == 'B') {

        //Añadimos la b
        comp->string[strlen(comp->string)] = c;
        c = sigCaracter();

        //Añadimos todos los numeros
        while ((isdigit(c))) {
            comp->string[strlen(comp->string)] = c;
            c = sigCaracter();
        }

        //Si solo es un 0 lo devovemos
    } else {
        e = 0;
    }

    //Aceptamos
    e = 0;
}

void masigualmasmas() { //Reconoce las cadenas += y ++
    comp->string[strlen(comp->string)] = c;
    c = sigCaracter();

    //Masigual
    if (c == '=') {
        comp->string[strlen(comp->string)] = c;
        c = sigCaracter();
        e = 0;

        //Masmas
    } else if (c == '+') {
        comp->string[strlen(comp->string)] = c;
        c = sigCaracter();
        e = 0;

    } else {
        e = 0;
    }
}

void igualigual() {
    if (c == '=') {
        comp->string[strlen(comp->string)] = c;
        c = sigCaracter();
    } else {
        e = 0;
    }

}

void einicial() { //Funcion que redirige a los demas automatas
    if (isalpha(c) || c == '_') { //Cadenas alfanumericas
        e = 2;
    } else if (isdigit(c) && c != '0') { //Numeros
        e = 3;
    } else if (c == '0') {
        e = 8;
    }/*else if (c == '.') {
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
        return comp;
    }
}

//Funcion que registra un ID en la tabla de simbolos
void registrarTabla(){
    
}