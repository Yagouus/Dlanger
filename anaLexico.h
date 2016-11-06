#include "tablaSimbolos.h"
#include "sistEntrada.h"
#include "gestErrores.h"

//Devuelve un lexema
compLex * sigCompLex();

////AUTOMATA////
void einicial();
void alfanum();
void comentarios();
void comillas();
void numeros();
void enteros();
void reales();
void binarios();
void nCientifica();
void masigualmasmas();
void igualigual();

////OTRAS FUNCIONES////
void registrarTabla();
void liberarLexico();