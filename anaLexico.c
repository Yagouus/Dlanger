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

                //Cadenas alfanuméricas
            case 1:                
                if (isalnum(c)) {
                    e = 2;
                } else if (isdigit(c)) {
                    e = 3;
                } else if (&c == ".") {
                    comp->string[strlen(comp->string)] = c;
                    e = 0;
                } else if (&c == ";") {
                    comp->string[strlen(comp->string)] = c;
                    e = 0;
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
                } else {
                    e = 0;
                }
                break;

                //Numeros
            case 3:
                return comp;
                break;

                //Estado de aceptacion
            case 0:
                return comp;
                break;

        }

        //c=sigCaracter();


    }


}