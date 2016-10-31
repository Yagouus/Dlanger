#include "tablaSimbolos.h"
#include "sistEntrada.h"
#include "gestErrores.h"

//Devuelve un lexema
compLex * sigCompLex();

//Registra una cadena en la tabla de simbolos
void registrarTabla();

//Autómatas
void alfanum();
void einicial();
void numeros();
void comentarios();
void igualigual();
void masigualmasmas();
void comillas();
void binarios();
void reales();
void enteros();
void nCientifica();
