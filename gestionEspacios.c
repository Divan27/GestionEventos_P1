#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestionEspacios.h"



/*
Acceso a variables globales definidas en gestionSitios.

- sitios: arreglo dinámico de punteros a estructuras Sitio.
- cantidadSitios: número total de sitios registrados.

Se utilizan para permitir que este arhcivo pueda trabajar con
los sitios previamente cargados en el sistema.
*/
extern Sitio **sitios;
extern int cantidadSitios;


/*
FUNCION: mostrarEspacios

OBJETIVO:
Mostrar todos los espacios registrados en el archivo espacios.txt.

ENTRADAS:
- No recibe parámetros.

SALIDAS:
- Imprime en consola la lista de sitios y sus espacios asociados.

RESTRICCIONES:
- El archivo espacios.txt debe existir.
- El formato esperado del archivo es:
  sitio,espacio,cantidad,lista de identificadores

DESCRIPCION:
Lee cada línea del archivo espacios.txt, separa sus campos
usando sscanf y muestra la información organizada.
Si el archivo no existe, indica que no hay espacios asignados.
*/
void mostrarEspacios() {
    FILE *file = fopen("espacios.txt", "r");

    if (file == NULL) {
        printf("\n*****Espacios no asignados*****\n");
        return;
    }

    char linea[300];

    printf("\nSITIOS Y ESPACIOS\n");
    printf("---------------------------------------\n");

    while (fgets(linea, sizeof(linea), file) != NULL) {

        linea[strcspn(linea, "\n")] = 0;

        char sitio[100], espacio[100], lista[200];
        int cantidad;

        int campos = sscanf(linea, "%[^,],%[^,],%d,%[^\n]",
                            sitio, espacio, &cantidad, lista);

        if (campos >= 3) {
            printf("Sitio de evento: %s\n", sitio);
            printf("Espacio de evento: %s\n", espacio);
            printf("Cantidad de espacios: %d\n", cantidad);

            if (campos == 4) {
                printf("Lista de espacios: %s\n", lista);
            }

            printf("\n");
        }
    }

    fclose(file);
}


/*
FUNCION: agregarEspacios

OBJETIVO:
Permitir agregar un nuevo conjunto de espacios a un sitio existente.

ENTRADAS:
- No recibe parámetros directamente.
- Solicita al usuario:
  - sitio al que se asignarán los espacios
  - nombre del espacio
  - cantidad de espacios
  - letra inicial de identificación

SALIDAS:
- Guarda el nuevo espacio en el archivo espacios.txt.

RESTRICCIONES:
- Debe existir al menos un sitio cargado desde antes.
- El nombre del espacio no debe contener espacios.
- La cantidad debe ser un número positivo.

DESCRIPCION:
El usuario selecciona un sitio registrado.
Luego ingresa el nombre del espacio, cantidad de lugares
y una letra inicial para generar identificadores automáticos

Los datos se guardan en el archivo en formato:
sitio,espacio,cantidad,A1,A2,A3
*/
void agregarEspacios() {

    if (cantidadSitios == 0) {
        printf("Debe cargar sitios primero.\n");
        return;
    }

    printf("\nSELECCIONE UN SITIO\n");
    printf("---------------------------------------\n");

    for (int i = 0; i < cantidadSitios; i++) {
        printf("%d - %s\n", i + 1, sitios[i]->nombre);
    }

    int opcion;
    printf("Opcion: ");
    scanf("%d", &opcion);

    if (opcion < 1 || opcion > cantidadSitios) {
        printf("Opcion invalida.\n");
        return;
    }

    Sitio *s = sitios[opcion - 1];

    char nombreEspacio[100];
    int cantidad;
    char inicial;

    printf("Nombre: ");
    scanf("%s", nombreEspacio);

    printf("Cantidad espacio: ");
    scanf("%d", &cantidad);

    printf("Letra inicial: ");
    scanf(" %c", &inicial);

    FILE *file = fopen("espacios.txt", "a");

    if (file == NULL) {
        printf("Error al abrir archivo.\n");
        return;
    }

    fprintf(file, "%s,%s,%d", s->nombre, nombreEspacio, cantidad);

    for (int i = 1; i <= cantidad; i++) {
        fprintf(file, ",%c%d", inicial, i);
    }

    fprintf(file, "\n");

    fclose(file);

    printf("Espacios agregados correctamente.\n");
}


/*
FUNCION: resetEspacios

OBJETIVO:
Eliminar un registro de espacios específico del archivo espacios.txt.

ENTRADAS:
- No recibe parámetros directamente.
- Solicita al usuario seleccionar el registro a eliminar.

SALIDAS:
- Elimina el registro seleccionado del archivo.

RESTRICCIONES:
- Debe existir al menos un registro en el archivo.
- El usuario debe confirmar la eliminación.

DESCRIPCION:
Muestra un menú con los registros disponibles.
El usuario selecciona el registro a eliminar y confirma si esta totalmente seguro.

El archivo se reescribe quitando la línea eliminada, utilizando un archivo temporal (temp.txt).
Finalmente se libera la memoria utilizada.
*/
void resetEspacios() {

    FILE *file = fopen("espacios.txt", "r");

    if (file == NULL) {
        printf("\n*****No existen espacios para eliminar*****\n");
        return;
    }

    char buffer[300];
    char **lineas = NULL;
    int total = 0;

    // lectura de archivo
    while (fgets(buffer, sizeof(buffer), file) != NULL) {

        char *linea = (char *)malloc(strlen(buffer) + 1);

        if (linea == NULL) {
            printf("Error de memoria.\n");
            fclose(file);
            return;
        }

        strcpy(linea, buffer);

        char **temp = realloc(lineas, (total + 1) * sizeof(char *));

        if (temp == NULL) {
            printf("Error de memoria.\n");
            fclose(file);
            return;
        }

        lineas = temp;
        lineas[total] = linea;
        total++;
    }

    fclose(file);

    if (total == 0) {
        printf("No hay espacios registrados.\n");
        return;
    }

    printf("\n--- ESPACIOS REGISTRADOS ---\n");

    for (int i = 0; i < total; i++) {

        char copia[300];
        strcpy(copia, lineas[i]);

        copia[strcspn(copia, "\n")] = 0;

        char sitio[100], espacio[100], lista[200];
        int cantidad;

        int campos = sscanf(copia, "%[^,],%[^,],%d,%[^\n]",
                            sitio, espacio, &cantidad, lista);

        printf("%d - %s - %s - %d",
               i + 1, sitio, espacio, cantidad);

        if (campos == 4) {
            printf(" - %s", lista);
        }

        printf("\n");
    }

    int opcion;
    printf("Seleccione el espacio a eliminar: ");
    scanf("%d", &opcion);

    if (opcion < 1 || opcion > total) {
        printf("Opcion invalida.\n");
        return;
    }

    char confirmacion;
    printf("Seguro que desea eliminar este registro? (s/n): ");
    scanf(" %c", &confirmacion);

    if (confirmacion != 's' && confirmacion != 'S') {

        printf("Operacion cancelada.\n");

        for (int i = 0; i < total; i++) {
            free(lineas[i]);
        }
        free(lineas);

        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");

    if (tempFile == NULL) {
        printf("Error al crear archivo temporal.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        if (i != (opcion - 1)) {
            fputs(lineas[i], tempFile);
        }
    }

    fclose(tempFile);

    remove("espacios.txt");
    rename("temp.txt", "espacios.txt");

    printf("Espacio eliminado correctamente.\n");

    for (int i = 0; i < total; i++) {
        free(lineas[i]);
    }
    free(lineas);
}


/*
FUNCION: gestionEspacios

OBJETIVO:
Mostrar el menú principal del módulo de gestión de espacios.

ENTRADAS:
- No recibe parámetros.

SALIDAS:
- Permite al usuario interactuar con las funciones.

RESTRICCIONES:
- La opción ingresada debe ser válida.

DESCRIPCION:
Implementa un menú repetitivo que permite:
1. Mostrar espacios registrados
2. Agregar nuevos espacios
3. Eliminar espacios de un sitio
4. Volver al menú principal
*/
void gestionEspacios() {
    int op;

    do {
        printf("\nGESTION DE ESPACIOS\n");
        printf("---------------------------------------\n");
        printf("1. Mostrar espacios\n");
        printf("2. Agregar espacios\n");
        printf("3. Reset espacios de un sitio\n");
        printf("4. Volver\n");
        printf("Seleccione: ");
        scanf("%d", &op);

        switch(op) {
            case 1:
                mostrarEspacios();
                break;
            case 2:
                agregarEspacios();
                break;
            case 3:
                resetEspacios();
                break;
            case 4:
                break;
            default:
                printf("Opcion invalida\n");
        }

    } while(op != 4);
}
