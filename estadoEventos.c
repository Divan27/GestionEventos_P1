#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estadoEventos.h"


typedef struct {
    char nombre[100];
    float costo;
    char disponibles[200];
    char vendidos[200];
} Sector;

typedef struct {
    char nombre[100];
    char productora[100];
    char fecha[100];
    char sitio[100];
    Sector *sectores;
    int totalSectores;
} Evento;


// Contar asientos
int contarAsientos(char lista[]) {
    if (strlen(lista) == 0) return 0;

    int count = 1;
    for (int i = 0; lista[i]; i++) {
        if (lista[i] == ',') count++;
    }
    return count;
}


// Leer campo hasta la coma
void leerCampo(char *linea, int *i, char *destino) {
    int j = 0;

    while (linea[*i] != ',' && linea[*i] != '\0') {
        destino[j++] = linea[*i];
        (*i)++;
    }

    destino[j] = '\0';

    if (linea[*i] == ',') (*i)++;
}


// Leer lista de asientos
void leerLista(char *linea, int *i, char *destino) {
    int j = 0;

    while (linea[*i] != '\0') {

        // detener si viene v:
        if (linea[*i] == 'v' && linea[*i + 1] == ':') {
            break;
        }

        // detener si viene nuevo sector
        if (linea[*i] != 'd' && linea[*i] != 'v') {
            char *p = &linea[*i];
            char *coma = strchr(p, ',');
            char *dosPuntos = strchr(p, ':');

            if (dosPuntos && (!coma || dosPuntos < coma)) {
                break;
            }
        }

        destino[j++] = linea[*i];
        (*i)++;
    }

    destino[j] = '\0';

    // limpiar coma inicial
    if (destino[0] == ',') {
        memmove(destino, destino + 1, strlen(destino));
    }

    if (linea[*i] == ',') (*i)++;
}


// Cargar eventos 
Evento *cargarEventos(int *totalEventos) {

    FILE *file = fopen("eventos.txt", "r");

    if (!file) {
        *totalEventos = 0;
        return NULL;
    }

    Evento *eventos = NULL;
    int countEventos = 0;
    char linea[500];

    while (fgets(linea, sizeof(linea), file)) {

        linea[strcspn(linea, "\n")] = 0;

        Evento ev;
        ev.sectores = NULL;
        ev.totalSectores = 0;

        int i = 0;

        // datos base
        leerCampo(linea, &i, ev.nombre);
        leerCampo(linea, &i, ev.productora);
        leerCampo(linea, &i, ev.fecha);
        leerCampo(linea, &i, ev.sitio);

        // sectores
        while (linea[i] != '\0') {

            char campo[200];
            leerCampo(linea, &i, campo);

            // detectar sector
            if (strchr(campo, ':') && campo[0] != 'd' && campo[0] != 'v') {

                char nombreSector[100];
                float costo;

                sscanf(campo, "%[^:]:%f", nombreSector, &costo);

                Sector *temp = realloc(ev.sectores,
                    (ev.totalSectores + 1) * sizeof(Sector));

                if (!temp) return NULL;

                ev.sectores = temp;

                Sector *s = &ev.sectores[ev.totalSectores];

                strcpy(s->nombre, nombreSector);
                s->costo = costo;
                strcpy(s->disponibles, "");
                strcpy(s->vendidos, "");

                // leer d: y v:
                while (linea[i] != '\0') {

                    if (linea[i] == 'd' && linea[i + 1] == ':') {
                        i += 2;

                        while (linea[i] == ' ') i++; // limpiar espacios

                        leerLista(linea, &i, s->disponibles);
                    }

                    if (linea[i] == 'v' && linea[i + 1] == ':') {
                        i += 2;

                        while (linea[i] == ' ') i++;

                        leerLista(linea, &i, s->vendidos);
                        break;
                    }

                    // detectar nuevo sector
                    if (strchr(&linea[i], ':') &&
                        linea[i] != 'd' && linea[i] != 'v') {
                        break;
                    }

                    i++;
                }

                ev.totalSectores++;
            }
        }

        Evento *tempEv = realloc(eventos,
            (countEventos + 1) * sizeof(Evento));

        if (!tempEv) return NULL;

        eventos = tempEv;
        eventos[countEventos] = ev;
        countEventos++;
    }

    fclose(file);

    *totalEventos = countEventos;
    return eventos;
}


// Mostrar detalle
void mostrarDetalleEvento(Evento *ev) {

    printf("\n--- DETALLE DEL EVENTO ---\n");
    printf("Nombre: %s\n", ev->nombre);
    printf("Productora: %s\n", ev->productora);
    printf("Fecha: %s\n", ev->fecha);
    printf("Sitio: %s\n", ev->sitio);

    printf("\n--- SECTORES ---\n");

    for (int i = 0; i < ev->totalSectores; i++) {

        Sector *s = &ev->sectores[i];

        int vendidos = contarAsientos(s->vendidos);
        float recaudado = vendidos * s->costo;

        printf("\nSector: %s\n", s->nombre);
        printf("Costo: %.2f\n", s->costo);
        printf("Recaudado: %.2f\n", recaudado);

        printf("Disponibles (d): %s\n",
               strlen(s->disponibles) ? s->disponibles : "Ninguno");

        printf("Vendidos (v): %s\n",
               strlen(s->vendidos) ? s->vendidos : "Ninguno");
    }
}


// Mostrar eventos
void mostrarEventos() {

    int totalEventos = 0;
    Evento *eventos = cargarEventos(&totalEventos);

    if (totalEventos == 0) {
        printf("\n*****No hay eventos registrados*****\n");
        return;
    }

    printf("\n--- EVENTOS DISPONIBLES ---\n");

    for (int i = 0; i < totalEventos; i++) {
        printf("%d - %s\n", i + 1, eventos[i].nombre);
    }

    int opcion;
    printf("Seleccione: ");
    scanf("%d", &opcion);

    if (opcion < 1 || opcion > totalEventos) {
        printf("Opcion invalida.\n");
        return;
    }

    mostrarDetalleEvento(&eventos[opcion - 1]);

    // 🔹 liberar memoria
    for (int i = 0; i < totalEventos; i++) {
        free(eventos[i].sectores);
    }
    free(eventos);
}


// Menú
void estadoEventos() {

    int op;

    do {
        printf("\n--- ESTADO DE EVENTOS ---\n");
        printf("1. Mostrar eventos\n");
        printf("2. Volver\n");
        printf("Seleccione: ");
        scanf("%d", &op);

        switch(op) {
            case 1:
                mostrarEventos();
                break;
            case 2:
                break;
            default:
                printf("Opcion invalida\n");
        }

    } while(op != 2);
}