#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestionEspacios.h"


// Acceso a variables globales de sitios
extern Sitio **sitios;
extern int cantidadSitios;

// Mostrar espacios desde archivo
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
            printf("Sitio de evento: %s\n",sitio);
            printf("Espacio de evento: %s\n",espacio);
            printf("Cantidad de espacios: %d\n",cantidad);

            if (campos == 4) {
                printf("Lista de espacios: %s\n", lista);
            }

            printf("\n");
        }
    }

    fclose(file);
}

// Agregar espacios a un sitio
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

    // 🔹 Guardar formato: sitio,espacio,cantidad,V1,V2...
    fprintf(file, "%s,%s,%d", s->nombre, nombreEspacio, cantidad);

    for (int i = 1; i <= cantidad; i++) {
        fprintf(file, ",%c%d", inicial, i);
    }

    fprintf(file, "\n");

    fclose(file);

    printf("Espacios agregados correctamente.\n");
}

void resetEspacios() {

    FILE *file = fopen("espacios.txt", "r");

    if (file == NULL) {
        printf("\n*****No existen espacios para eliminar*****\n");
        return;
    }

    char lineas[200][300];
    int total = 0;

    // 🔹 Leer todas las líneas
    while (fgets(lineas[total], sizeof(lineas[total]), file) != NULL) {
        total++;
    }

    fclose(file);

    if (total == 0) {
        printf("No hay espacios registrados.\n");
        return;
    }

    // Mostrar como menú numerado
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

    // Selección
    int opcion;
    printf("Seleccione el espacio a eliminar: ");
    scanf("%d", &opcion);

    if (opcion < 1 || opcion > total) {
        printf("Opcion invalida.\n");
        return;
    }

    // Confirmación
    char confirmacion;
    printf("Seguro que desea eliminar este registro? (s/n): ");
    scanf(" %c", &confirmacion);

    if (confirmacion != 's' && confirmacion != 'S') {
        printf("Operacion cancelada.\n");
        return;
    }

    // Reescribir archivo sin la línea seleccionada
    FILE *temp = fopen("temp.txt", "w");

    if (temp == NULL) {
        printf("Error al crear archivo temporal.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        if (i != (opcion - 1)) {
            fputs(lineas[i], temp);
        }
    }

    fclose(temp);

    remove("espacios.txt");
    rename("temp.txt", "espacios.txt");

    printf("Espacio eliminado correctamente.\n");
}

// Menú de gestión de espacios
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