#include "definiciones.h"
#include "gestErrores.h"

////TIPOS DE DATO////
//Estructura de un componente lexico
typedef struct {
    int id; //Contiene un id
    char* string; //Y el lexema en si
} compLex;

//Nodo del arbol
struct nodo {
    compLex* lexema; //Contiene un componente lexico
    struct nodo *izq, *der; //Y punteros a sus subarboles
};

typedef struct nodo * arbol;

////ARBOL////
void crea();
void destruye();
unsigned esVacio();
void inserta(arbol* A, compLex* comp);
compLex* busca(arbol A, compLex *comp);
arbol izq();
arbol der();
void imprimeElemento(arbol* A);

////TABLA SIMBOLOS////
void crearTabla();
void destruyeTabla();
void inicializa();
int buscarEnTabla(compLex* comp);
void insertarEnTabla(compLex* comp);
void insertarPalReservada(char* lexema, int id);
void imprimeTabla();
void asignarID();
void insertaElemento(compLex* comp);

