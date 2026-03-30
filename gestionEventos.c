#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestionEventos.h"
#include "utils.h"

/*
Acceso a variables globales definidas en el archivo de gestionSitios.

- sitios: arreglo dinámico de estructuras Sitio cargadas en memoria.
- cantidadSitios: número total de sitios disponibles.

Permite asociar eventos a un sitio específico registrado.
*/
extern Sitio **sitios;
extern int cantidadSitios;


/*
Estructura auxiliar que representa un espacio dentro de un sitio.

Campos:
- nombre: nombre del espacio
- asientos: lista de asientos disponibles en formato texto
*/
typedef struct {
    char nombre[100];
    char asientos[200]; 
} Espacio;


/*
FUNCION: obtenerEspacios

OBJETIVO:
Obtener todos los espacios asociados a un sitio específico desde el archivo espacios.txt.

ENTRADAS:
- nombreSitio: nombre del sitio del cual se desean obtener los espacios.
- totalEspacios: puntero donde se almacenará la cantidad de espacios encontrados.

SALIDAS:
- Retorna un arreglo dinámico de estructuras Espacio.
- totalEspacios contiene la cantidad total de espacios encontrados.

RESTRICCIONES:
- El archivo espacios.txt debe existir.
- El formato del archivo debe ser:
  sitio,espacio,cantidad,A1,A2,A3,...

DESCRIPCION:
Lee el archivo espacios.txt línea por línea.
Filtra únicamente los espacios que pertenecen al sitio indicado.
Guardar los resultados en memoria dinámica usando realloc.
Cada espacio incluye su nombre y la lista de asientos disponibles.
*/
Espacio *obtenerEspacios(char nombreSitio[], int *totalEspacios) {

    FILE *file = fopen("espacios.txt", "r");

    if (file == NULL) {
        printf("No existen espacios registrados.\n");
        *totalEspacios = 0;
        return NULL;
    }

    char linea[300];
    Espacio *espacios = NULL;
    int count = 0;

    while (fgets(linea, sizeof(linea), file) != NULL) {

        char sitio[100], espacio[100], lista[200];
        int cantidad;

        int campos = sscanf(linea, "%[^,],%[^,],%d,%[^\n]",
                            sitio, espacio, &cantidad, lista);

        // verificar si el espacio pertenece al sitio seleccionado
        if (strcmp(sitio, nombreSitio) == 0) {

            Espacio *temp = realloc(espacios, (count + 1) * sizeof(Espacio));

            if (temp == NULL) {
                fclose(file);
                return NULL;
            }

            espacios = temp;

            strcpy(espacios[count].nombre, espacio);

            if (campos == 4) {
                strcpy(espacios[count].asientos, lista);
            } else {
                strcpy(espacios[count].asientos, "");
            }

            count++;
        }
    }

    fclose(file);

    *totalEspacios = count;
    return espacios;
}


/*
FUNCION: crearEvento

OBJETIVO:
Permitir al usuario registrar un nuevo evento en el sistema.

ENTRADAS:
- No recibe parámetros directamente.
- Solicita al usuario:
  - nombre del evento
  - productora
  - fecha
  - sitio donde se realizará
  - costo por cada espacio

SALIDAS:
- Guarda el evento en el archivo eventos.txt.

RESTRICCIONES:
- Debe existir al menos un sitio registrado.
- El sitio seleccionado debe tener espacios asignados.
- El archivo eventos.txt debe poder abrirse en modo escritura.

DESCRIPCION:
El usuario ingresa los datos básicos del evento.
Luego selecciona un sitio disponible.

El sistema obtiene automáticamente los espacios asociados al sitio
desde el archivo espacios.txt.

Para cada espacio, el usuario define el costo.
El sistema guarda el evento en formato estructurado:

nombre,productora,fecha,sitio,sector:costo,d:listaAsientos,v:

*/
void crearEvento() {

    if (cantidadSitios == 0) {
        printf("Debe cargar sitios primero.\n");
        return;
    }

    char nombre[100], productora[100], fecha[50];

    printf("Nombre del evento: ");
    scanf(" %[^\n]", nombre);

    printf("Productora: ");
    scanf(" %[^\n]", productora);

    printf("Fecha: ");
    scanf(" %[^\n]", fecha);

    // mostrar sitios disponibles
    printf("\n--- Seleccione un sitio ---\n");

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

    // obtener espacios del sitio
    int totalEspacios = 0;

    Espacio *espacios = obtenerEspacios(s->nombre, &totalEspacios);

    if (totalEspacios == 0) {
        printf("Este sitio no tiene espacios asignados.\n");
        return;
    }

    FILE *file = fopen("eventos.txt", "a");

    if (file == NULL) {
        printf("Error al guardar evento.\n");
        return;
    }

    // guardar datos base del evento
    fprintf(file, "%s,%s,%s,%s",
        nombre, productora, fecha, s->nombre);

    // guardar sectores y costos
    for (int i = 0; i < totalEspacios; i++) {

        float costo;

        printf("Costo para %s: ", espacios[i].nombre);
        scanf("%f", &costo);

        // guardar nombre del sector y costo
        fprintf(file, ",%s:%.2f", espacios[i].nombre, costo);

        // guardar lista de asientos disponibles
        fprintf(file, ",d:%s", espacios[i].asientos);

        // lista de vendidos inicia vacía
        fprintf(file, ",v:");
    }

    fprintf(file, "\n");

    fclose(file);

    free(espacios);

    printf("\nEvento creado correctamente.\n");
}


/*
FUNCION: gestionEventos

OBJETIVO:
Mostrar el menú principal del módulo de gestión de eventos.

ENTRADAS:
- No recibe parámetros.

SALIDAS:
- Permite al usuario crear nuevos eventos.

RESTRICCIONES:
- La opción ingresada debe ser válida.

DESCRIPCION:
Implementa un menú repetitivo que permite:
1. Crear un evento nuevo
2. Volver al menú principal
*/
void gestionEventos() {

    int op;

    do {
        printf("\n--- GESTION DE EVENTOS ---\n");
        printf("1. Crear evento\n");
        printf("2. Volver\n");
        printf("Seleccione: ");
        scanf("%d", &op);

        switch(op) {

            case 1:
                crearEvento();
                break;

            case 2:
                break;

            default:
                printf("Opcion invalida\n");
        }

    } while(op != 2);
}