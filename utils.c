#include <stdio.h>
#include <string.h>
#include "utils.h"

/*
------------------------------------------------------------
FUNCION: contarAsientos
OBJETIVO:
Contar la cantidad de asientos dentro de una cadena separada
por comas.

ENTRADAS:
- lista: cadena de caracteres con asientos separados por coma
  Ejemplo: "A1,A2,A3"

SALIDAS:
- Retorna un entero con la cantidad de asientos.

RESTRICCIONES:
- La cadena debe estar correctamente formada.
- No debe contener comas consecutivas.
------------------------------------------------------------
*/
int contarAsientos(char lista[]) {
    if (strlen(lista) == 0) return 0;

    int count = 1;

    for (int i = 0; lista[i]; i++) {
        if (lista[i] == ',') count++;
    }

    return count;
}


/*
------------------------------------------------------------
FUNCION: convertirFecha
OBJETIVO:
Convertir una fecha en formato DD/MM/AA a un valor numérico
para facilitar comparaciones.

ENTRADAS:
- fecha: cadena con formato "DD/MM/AA"

SALIDAS:
- Retorna un entero con formato AAAAMMDD.

RESTRICCIONES:
- La fecha debe estar en el formato correcto.
- No valida fechas inválidas (ej: 32/13/99).
------------------------------------------------------------
*/
int convertirFecha(char fecha[]) {
    int d, m, a;

    sscanf(fecha, "%d/%d/%d", &d, &m, &a);

    return a * 10000 + m * 100 + d;
}


/*
------------------------------------------------------------
FUNCION: leerCampo
OBJETIVO:
Extraer un campo de una línea de texto hasta encontrar una
coma o el final de la cadena.

ENTRADAS:
- linea: cadena completa que se está procesando.
- i: puntero al índice actual de lectura.
- destino: arreglo donde se almacenará el campo leído.

SALIDAS:
- Guarda el campo leído en "destino".
- Actualiza la posición del índice "i".

RESTRICCIONES:
- El destino debe tener suficiente espacio.
- La línea debe estar en formato CSV válido.
------------------------------------------------------------
*/
void leerCampo(char *linea, int *i, char *destino) {
    int j = 0;

    while (linea[*i] != ',' && linea[*i] != '\0') {
        destino[j++] = linea[*i];
        (*i)++;
    }

    destino[j] = '\0';

    if (linea[*i] == ',') (*i)++;
}


/*
------------------------------------------------------------
FUNCION: leerLista
OBJETIVO:
Leer una lista de elementos (asientos) desde una cadena,
deteniéndose cuando inicia otra sección (v:) o al final.

ENTRADAS:
- linea: cadena completa.
- i: puntero al índice actual.
- destino: arreglo donde se almacenará la lista.

SALIDAS:
- Guarda la lista de asientos en "destino".
- Actualiza la posición del índice "i".

RESTRICCIONES:
- Funciona bajo el formato definido del archivo eventos.txt.
- Se detiene únicamente al encontrar "v:" o fin de cadena.
------------------------------------------------------------
*/
void leerLista(char *linea, int *i, char *destino) {
    int j = 0;

    while (linea[*i] != '\0') {

        // detener si inicia sección de vendidos
        if (linea[*i] == 'v' && linea[*i + 1] == ':') break;

        destino[j++] = linea[*i];
        (*i)++;
    }

    destino[j] = '\0';

    // eliminar coma inicial si existe
    if (destino[0] == ',') {
        memmove(destino, destino + 1, strlen(destino));
    }

    if (linea[*i] == ',') (*i)++;
}