#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estadoEventos.h"
#include "utils.h"

/*
Estructura que representa un sector dentro de un evento.

Campos:
- nombre: nombre del sector 
- costo: precio del asiento en ese sector
- disponibles: lista de asientos disponibles separados por coma
- vendidos: lista de asientos vendidos separados por coma
*/
typedef struct {
    char nombre[100];
    float costo;
    char disponibles[200];
    char vendidos[200];
} Sector;


/*
Estructura que representa un evento.

Campos:
- nombre: nombre del evento
- productora: empresa productora del evento
- fecha: fecha del evento
- sitio: lugar donde se realiza el evento
- sectores: arreglo dinámico de sectores del evento
- totalSectores: cantidad total de sectores
*/
typedef struct {
    char nombre[100];
    char productora[100];
    char fecha[100];
    char sitio[100];
    Sector *sectores;
    int totalSectores;
} Evento;



/*
FUNCION: cargarEventos

OBJETIVO:
Leer el archivo eventos.txt y cargar todos los eventos.

ENTRADAS:
- totalEventos: puntero donde se almacenará la cantidad de eventos cargados.

SALIDAS:
- Retorna un arreglo dinámico de estructuras Evento.
- totalEventos contendrá la cantidad total de eventos encontrados.

RESTRICCIONES:
- El archivo eventos.txt debe existir.
- El formato del archivo debe cumplir la estructura:
  nombre,productora,fecha,sitio,sector:costo,d:lista,v:lista,...
Para así poder ser analizada correctamente

DESCRIPCION:
Lee línea por línea el archivo de eventos, separa los campos principales
y luego procesa cada sector con sus asientos disponibles y vendidos.
Estamos utilizando memoria dinámica realloc para almacenar los eventos y sectores.
*/
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

        // leer datos base del evento
        leerCampo(linea, &i, ev.nombre);
        leerCampo(linea, &i, ev.productora);
        leerCampo(linea, &i, ev.fecha);
        leerCampo(linea, &i, ev.sitio);

        // leer sectores
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

                // leer listas de asientos
                while (linea[i] != '\0') {

                    if (linea[i] == 'd' && linea[i + 1] == ':') {
                        i += 2;

                        while (linea[i] == ' ') i++;

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


/*
FUNCION: mostrarDetalleEvento

OBJETIVO:
Mostrar en pantalla toda la información detallada de un evento.

ENTRADAS:
- ev: puntero a la estructura Evento que se desea mostrar.

SALIDAS:
- Imprime en consola el detalle del evento y sus sectores.

RESTRICCIONES:
- El evento debe haber sido cargado previamente.

DESCRIPCION:
Muestra información general del evento y calcula la recaudación
de cada sector en base a la cantidad de asientos vendidos.
*/
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


/*
FUNCION: mostrarEventos

OBJETIVO:
Mostrar la lista de eventos disponibles y permitir seleccionar uno.

ENTRADAS:
- No recibe parámetros.

SALIDAS:
- Muestra en consola la lista de eventos y su detalle.

RESTRICCIONES:
- El archivo eventos.txt debe existir.
- Debe existir al menos un evento registrado.

DESCRIPCION:
Carga los eventos desde el archivo, muestra un menú de selección
y permite ver el detalle de un evento específico.
También libera la memoria dinámica utilizada.
*/
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

    // liberar memoria dinámica
    for (int i = 0; i < totalEventos; i++) {
        free(eventos[i].sectores);
    }
    free(eventos);
}


/*
FUNCION: estadoEventos

OBJETIVO:
Mostrar el menú principal del módulo de estado de eventos.

ENTRADAS:
- No recibe parámetros.

SALIDAS:
- Permite al usuario navegar entre opciones del menú.

RESTRICCIONES:
- La opción ingresada debe ser válida.

DESCRIPCION:
Implementa un menú repetitivo que permite consultar
los eventos registrados o volver al menú principal.
*/
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
