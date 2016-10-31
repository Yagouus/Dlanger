#include "definiciones.h"

//Estructura de un componente lexico
//Contiene un id
//Y el lexema en si
typedef struct {
    int id;
    char* string;
} compLex;

//Nodo del arbol
//Contiene un componente lexico
//Y punteros a sus subarboles
struct nodo {
    compLex* lexema;
    struct nodo *izq, *der;
};

typedef struct nodo * arbol;

//Funciones arbol
void crea();
void destruye();
unsigned esVacio();
void inserta(arbol* A, compLex* comp);
compLex* busca(arbol A, compLex *comp);
arbol izq();
arbol der();

//Funciones tabla
void crearTabla();
void destruyeTabla();
void inicializa();
int buscarEnTabla(compLex* comp);
void insertarEnTabla(compLex* comp);
void insertarPalReservada(char* lexema, int id);
void imprimeTabla();
void insertaElemento(compLex* comp);

