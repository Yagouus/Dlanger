#include <stdio.h>
#include <stdlib.h>
#include "anaLexico.h"


void analisisSintactico(){
    
    //Pedimos el primer componente lexico
    compLex* comp = sigCompLex();
    
    //Iteramos por todos los componentes
    while(comp){
        
        //Imprimimos el lexema e id
        printf("<%d> <%s>\n", comp->id, comp->string);
        
        //Pedimos siguiente componente
        comp = sigCompLex();
    }
    
}
