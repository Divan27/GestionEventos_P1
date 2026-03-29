#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestionSitios.h"

#define MAX 200


Sitio **sitios = NULL; // 🔹 arreglo de punteros
int cantidadSitios = 0;

// 🔹 Verificar duplicado por nombre
int existeSitio(char nombre[]) {
    for (int i = 0; i < cantidadSitios; i++) {
        if (strcmp(sitios[i]->nombre, nombre) == 0) {
            return 1;
        }
    }
    return 0;
}

// Validar web
int validarWeb(char web[]) {
    return (strncmp(web, "www.", 4) == 0 && strchr(web, '.') != NULL);
}

// Liberar memoria
void liberarMemoria() {
    for (int i = 0; i < cantidadSitios; i++) {
        free(sitios[i]); // liberar cada struct
    }
    free(sitios); // liberar arreglo de punteros
    sitios = NULL;
    cantidadSitios = 0;
}

// Cargar desde archivo
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

        int campos = sscanf(linea, "%[^,],%[^,],%[^\n]", nombre, ubicacion, web);

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

        // crecer arreglo de punteros
        Sitio **temp = realloc(sitios, (cantidadSitios + 1) * sizeof(Sitio *));
        if (temp == NULL) {
            printf("Error de memoria.\n");
            fclose(archivo);
            return;
        }

        sitios = temp;

        // reservar memoria para el nuevo struct
        sitios[cantidadSitios] = (Sitio *)malloc(sizeof(Sitio));
        if (sitios[cantidadSitios] == NULL) {
            printf("Error de memoria.\n");
            fclose(archivo);
            return;
        }

        //  usar ->
        strcpy(sitios[cantidadSitios]->nombre, nombre);
        strcpy(sitios[cantidadSitios]->ubicacion, ubicacion);
        strcpy(sitios[cantidadSitios]->web, web);

        cantidadSitios++;
    }

    fclose(archivo);
    printf("\n*****Carga completada*****\n");
}

// Listar sitios
void listarSitios() {
    if (cantidadSitios == 0) {
        printf("No hay sitios registrados.\n");
        return;
    }

    printf("\nSITIOS DE EVENTOS\n");
    printf("---------------------------------------\n");

    for (int i = 0; i < cantidadSitios; i++) {
        printf("Nombre del sitio: %s",sitios[i]->nombre);
        printf("\nUbicacion del sitio: %s",sitios[i]->ubicacion);
        printf("\nSitio web del sitio: %s\n\n",sitios[i]->web);
    }
}

// Menú
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
