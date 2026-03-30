#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consultaEvento.h"
#include "utils.h"

/*
------------------------------------------------------------
ESTRUCTURA: Sector
OBJETIVO:
Representar un sector dentro de un evento, incluyendo su
nombre, costo y lista de asientos disponibles.

ATRIBUTOS:
- nombre: nombre del sector.
- costo: precio por asiento.
- disponibles: lista de asientos disponibles separados por coma.

RESTRICCIONES:
- El arreglo de disponibles debe respetar el formato CSV.
------------------------------------------------------------
*/
typedef struct {
    char nombre[100];
    float costo;
    char disponibles[200];
} Sector;


/*
------------------------------------------------------------
ESTRUCTURA: Evento
OBJETIVO:
Representar un evento con su información general y los
sectores asociados.

ATRIBUTOS:
- nombre: nombre del evento.
- productora: empresa organizadora.
- fecha: fecha del evento (DD/MM/AA).
- sitio: lugar del evento.
- sectores: arreglo dinámico de sectores.
- totalSectores: cantidad de sectores.

RESTRICCIONES:
- La memoria de sectores debe manejarse dinámicamente.
------------------------------------------------------------
*/
typedef struct {
    char nombre[100];
    char productora[100];
    char fecha[20];
    char sitio[100];
    Sector *sectores;
    int totalSectores;
} Evento;


/*
------------------------------------------------------------
FUNCION: cargarEventos
OBJETIVO:
Leer el archivo "eventos.txt" y cargar los eventos en memoria
utilizando estructuras dinámicas.

ENTRADAS:
- total: puntero donde se almacenará la cantidad de eventos.

SALIDAS:
- Retorna un arreglo dinámico de eventos.

RESTRICCIONES:
- El archivo debe existir y estar en formato correcto.
- Utiliza realloc para crecimiento dinámico.
- La memoria debe ser liberada posteriormente.
------------------------------------------------------------
*/
static Evento *cargarEventos(int *total) {

    FILE *file = fopen("eventos.txt", "r");
    if (!file) {
        *total = 0;
        return NULL;
    }

    Evento *eventos = NULL;
    int count = 0;
    char linea[500];

    while (fgets(linea, sizeof(linea), file)) {

        linea[strcspn(linea, "\n")] = 0;

        Evento ev;
        ev.sectores = NULL;
        ev.totalSectores = 0;

        int i = 0;

        // Datos principales
        leerCampo(linea, &i, ev.nombre);
        leerCampo(linea, &i, ev.productora);
        leerCampo(linea, &i, ev.fecha);
        leerCampo(linea, &i, ev.sitio);

        // Procesar sectores
        while (linea[i] != '\0') {

            char campo[200];
            leerCampo(linea, &i, campo);

            // detectar sector (Nombre:precio)
            if (strchr(campo, ':') &&
                !(campo[0] == 'd' && campo[1] == ':') &&
                !(campo[0] == 'v' && campo[1] == ':')) {

                char nombreSector[100];
                float costo;

                sscanf(campo, "%[^:]:%f", nombreSector, &costo);

                // 🔹 crecer arreglo
                Sector *temp = realloc(ev.sectores,
                    (ev.totalSectores + 1) * sizeof(Sector));

                if (!temp) return NULL;

                ev.sectores = temp;

                Sector *s = &ev.sectores[ev.totalSectores];

                strcpy(s->nombre, nombreSector);
                s->costo = costo;
                strcpy(s->disponibles, "");

                // 🔹 leer disponibles (d:)
                if (linea[i] == 'd' && linea[i + 1] == ':') {
                    i += 2;
                    leerLista(linea, &i, s->disponibles);
                }

                // 🔹 saltar vendidos (v:)
                if (linea[i] == 'v' && linea[i + 1] == ':') {

                    i += 2;

                    while (linea[i] != '\0') {

                        // si llega a coma → avanzar y salir
                        if (linea[i] == ',') {
                            i++;
                            break;
                        }

                        // detectar nuevo sector manualmente
                        int k = i;

                        while (linea[k] != ':' &&
                               linea[k] != '\0' &&
                               linea[k] != ',') {
                            k++;
                        }

                        if (linea[k] == ':') {

                            if (!(linea[i] == 'd' && linea[i + 1] == ':') &&
                                !(linea[i] == 'v' && linea[i + 1] == ':')) {

                                break; // nuevo sector
                            }
                        }

                        i++;
                    }
                }

                ev.totalSectores++;
            }
        }

        // agregar evento
        Evento *tempEv = realloc(eventos, (count + 1) * sizeof(Evento));
        if (!tempEv) return NULL;

        eventos = tempEv;
        eventos[count++] = ev;
    }

    fclose(file);

    *total = count;
    return eventos;
}


/*
------------------------------------------------------------
FUNCION: mostrarEvento
OBJETIVO:
Mostrar en pantalla la información detallada de un evento,
incluyendo sus sectores y asientos disponibles.

ENTRADAS:
- ev: puntero a la estructura Evento a mostrar.

SALIDAS:
- Imprime la información en consola.

RESTRICCIONES:
- El puntero debe ser válido.
- Los datos deben estar previamente cargados.
------------------------------------------------------------
*/
void mostrarEvento(Evento *ev) {

    printf("\n--- EVENTO ---\n");
    printf("Nombre: %s\n", ev->nombre);
    printf("Productora: %s\n", ev->productora);
    printf("Sitio: %s\n", ev->sitio);
    printf("Fecha: %s\n", ev->fecha);

    printf("\n--- SECTORES ---\n");

    for (int i = 0; i < ev->totalSectores; i++) {

        Sector *s = &ev->sectores[i];

        printf("\nSector: %s\n", s->nombre);
        printf("Costo: %.2f\n", s->costo);

        printf("Disponibles: %s\n",
               strlen(s->disponibles) ? s->disponibles : "Ninguno");
    }
}


/*
------------------------------------------------------------
FUNCION: consultaEvento
OBJETIVO:
Permitir al usuario consultar eventos futuros a partir de una
fecha ingresada y visualizar su información detallada.

ENTRADAS:
- Entrada por teclado de la fecha (DD/MM/AA).

SALIDAS:
- Muestra lista de eventos filtrados.
- Muestra detalle del evento seleccionado.

RESTRICCIONES:
- El archivo eventos.txt debe existir.
- La fecha debe estar en formato válido.
- Se debe liberar la memoria al finalizar.
------------------------------------------------------------
*/
void consultaEvento() {

    char fechaUsuario[20];

    printf("Ingrese fecha (DD/MM/AA): ");
    scanf("%s", fechaUsuario);

    int fechaBase = convertirFecha(fechaUsuario);

    int total = 0;
    Evento *eventos = cargarEventos(&total);

    if (total == 0) {
        printf("No hay eventos.\n");
        return;
    }

    printf("\n--- EVENTOS DISPONIBLES ---\n");

    int indices[100];
    int count = 0;

    for (int i = 0; i < total; i++) {

        if (convertirFecha(eventos[i].fecha) >= fechaBase) {
            printf("%d - %s (%s)\n",
                   count + 1,
                   eventos[i].nombre,
                   eventos[i].fecha);

            indices[count++] = i;
        }
    }

    if (count == 0) {
        printf("No hay eventos futuros.\n");
        return;
    }

    int op;
    printf("Seleccione: ");
    scanf("%d", &op);

    if (op < 1 || op > count) {
        printf("Opcion invalida.\n");
        return;
    }

    mostrarEvento(&eventos[indices[op - 1]]);

    // liberar memoria
    for (int i = 0; i < total; i++) {
        free(eventos[i].sectores);
    }
    free(eventos);
}