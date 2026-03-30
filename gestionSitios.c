#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestionSitios.h"

#define MAX 200


/*
Variables globales que almacenan los sitios cargados en memoria.

- sitios: arreglo dinámico de punteros a estructuras Sitio.
- cantidadSitios: cantidad total de sitios almacenados.

Se utilizan en otros archivos mediante la palabra reservada extern.
*/
Sitio **sitios = NULL; 
int cantidadSitios = 0;


/*
FUNCION: existeSitio

OBJETIVO:
Verificar si ya existe un sitio registrado con el mismo nombre.

ENTRADAS:
- nombre: nombre del sitio que se desea validar.

SALIDAS:
- Retorna 1 si el sitio ya existe.
- Retorna 0 si el sitio no existe.

RESTRICCIONES:
- La comparación se realiza de forma exacta (sensible a mayúsculas/minúsculas).

DESCRIPCION:
Recorre el arreglo dinámico de sitios y compara el nombre recibido
con los nombres existentes usando strcmp.
*/
int existeSitio(char nombre[]) {

    for (int i = 0; i < cantidadSitios; i++) {

        if (strcmp(sitios[i]->nombre, nombre) == 0) {
            return 1;
        }
    }

    return 0;
}


/*
FUNCION: validarWeb

OBJETIVO:
Validar si la dirección web cumple con un formato básico.

ENTRADAS:
- web: cadena que representa la dirección web del sitio.

SALIDAS:
- Retorna 1 si la web cumple el formato.
- Retorna 0 si la web no cumple el formato.

RESTRICCIONES:
- El formato válido debe iniciar con "www."
- Debe contener al menos un punto adicional.

DESCRIPCION:
Utiliza strncmp para validar el prefijo "www."
y strchr para verificar la existencia de un punto.
*/
int validarWeb(char web[]) {

    return (strncmp(web, "www.", 4) == 0 && strchr(web, '.') != NULL);
}


/*
FUNCION: liberarMemoria

OBJETIVO:
Liberar toda la memoria dinámica utilizada para almacenar sitios.

ENTRADAS:
- No recibe parámetros.

SALIDAS:
- Libera la memoria del arreglo de sitios.
- Reinicia las variables globales.

RESTRICCIONES:
- Debe llamarse antes de volver a cargar sitios para evitar fugas de memoria.

DESCRIPCION:
Libera cada estructura Sitio almacenada en memoria
y luego libera el arreglo de punteros.
Reinicia la cantidad de sitios a 0.
*/
void liberarMemoria() {

    for (int i = 0; i < cantidadSitios; i++) {
        free(sitios[i]);
    }

    free(sitios);

    sitios = NULL;
    cantidadSitios = 0;
}


/*
FUNCION: cargarSitiosLote

OBJETIVO:
Cargar múltiples sitios desde un archivo de texto.

ENTRADAS:
- Solicita al usuario la ruta del archivo.

SALIDAS:
- Almacena los sitios válidos en memoria dinámica.

RESTRICCIONES:
- El archivo debe existir.
- El formato del archivo debe ser:
  nombre,ubicacion,web

DESCRIPCION:
Primero libera la memoria existente para evitar duplicados.
Luego abre el archivo indicado por el usuario y lee cada línea.

Valida:
- que el sitio no esté duplicado
- que la web tenga formato válido

*/
void cargarSitiosLote() {

    char ruta[100];

    // limpiar memoria antes de recargar
    liberarMemoria();

    printf("Ingrese la ruta del archivo: ");

    int c;

    while ((c = getchar()) != '\n' && c != EOF);

    fgets(ruta, sizeof(ruta), stdin);

    ruta[strcspn(ruta, "\n")] = 0;

    FILE *archivo = fopen(ruta, "r");

    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    char linea[300];

    char nombre[100], ubicacion[100], web[100];

    while (fgets(linea, sizeof(linea), archivo) != NULL) {

        linea[strcspn(linea, "\n")] = 0;

        int campos = sscanf(linea, "%[^,],%[^,],%[^\n]",
                            nombre, ubicacion, web);

        if (campos < 2) {
            printf("Linea invalida, se omite.\n");
            continue;
        }

        if (campos == 2) {
            strcpy(web, "");
        }

        if (existeSitio(nombre)) {
            printf("Sitio duplicado (%s), se omite.\n", nombre);
            continue;
        }

        if (strlen(web) > 0 && !validarWeb(web)) {
            printf("Web invalida (%s), se omite.\n", web);
            continue;
        }

        Sitio **temp = realloc(sitios,
            (cantidadSitios + 1) * sizeof(Sitio *));

        if (temp == NULL) {
            printf("Error de memoria.\n");
            fclose(archivo);
            return;
        }

        sitios = temp;

        sitios[cantidadSitios] =
            (Sitio *)malloc(sizeof(Sitio));

        if (sitios[cantidadSitios] == NULL) {
            printf("Error de memoria.\n");
            fclose(archivo);
            return;
        }

        strcpy(sitios[cantidadSitios]->nombre, nombre);
        strcpy(sitios[cantidadSitios]->ubicacion, ubicacion);
        strcpy(sitios[cantidadSitios]->web, web);

        cantidadSitios++;
    }

    fclose(archivo);

    printf("\n*****Carga completada*****\n");
}


/*
FUNCION: listarSitios

OBJETIVO:
Mostrar todos los sitios cargados en memoria.

ENTRADAS:
- No recibe parámetros.

SALIDAS:
- Imprime en consola la información de los sitios.

RESTRICCIONES:
- Debe existir al menos un sitio cargado.

DESCRIPCION:
Recorre el arreglo dinámico de sitios
y muestra el nombre, ubicación y página web de cada uno.
*/
void listarSitios() {

    if (cantidadSitios == 0) {

        printf("No hay sitios registrados.\n");

        return;
    }

    printf("\nSITIOS DE EVENTOS\n");

    printf("---------------------------------------\n");

    for (int i = 0; i < cantidadSitios; i++) {

        printf("Nombre del sitio: %s",
            sitios[i]->nombre);

        printf("\nUbicacion del sitio: %s",
            sitios[i]->ubicacion);

        printf("\nSitio web del sitio: %s\n\n",
            sitios[i]->web);
    }
}


/*
FUNCION: gestionSitios

OBJETIVO:
Mostrar el menú principal del módulo de gestión de sitios.

ENTRADAS:
- No recibe parámetros.

SALIDAS:
- Permite al usuario cargar o listar sitios.

RESTRICCIONES:
- La opción ingresada debe ser válida.

DESCRIPCION:
Implementa un menú repetitivo que permite:
1. Cargar sitios desde archivo
2. Listar sitios
3. Volver al menú principal
*/
void gestionSitios() {

    int op;

    do {

        printf("\nGESTION DE SITIOS\n");

        printf("---------------------------------------\n");

        printf("1. Cargar sitios desde archivo\n");

        printf("2. Listar sitios\n");

        printf("3. Volver\n");

        printf("Seleccione: ");

        scanf("%d", &op);

        switch(op) {

            case 1:
                cargarSitiosLote();
                break;

            case 2:
                listarSitios();
                break;

            case 3:
                break;

            default:
                printf("Opcion invalida\n");
        }

    } while(op != 3);
}
