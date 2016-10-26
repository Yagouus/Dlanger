typedef struct {
    int id;
    char* string;
} compLex;

struct nodo {
    compLex* lexema;
    struct nodo *izq, *der;
};

typedef struct nodo * arbol;

void crea();
void destruye();
unsigned esVacio();
void inserta(arbol* A, compLex* comp);
compLex* busca(arbol A, compLex *comp);
arbol izq();
arbol der();
void inicializa();


