#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "listaFacturas.h"

typedef struct {
    int id;
    char fecha[20];
    char cedula[20];
    char cliente[100];
    char evento[100];
    float subtotal;
} Factura;

/*------------------------------------------------------------
FUNCION: cargarFacturas
OBJETIVO:
Cargar facturas desde archivo a memoria dinámica.
ENTRADAS:
- total: referencia para devolver cantidad
SALIDA:
- arreglo dinámico de Factura
------------------------------------------------------------*/
Factura *cargarFacturas(int *total) {

    FILE *file = fopen("factura.txt", "r");

    if (!file) {
        *total = 0;
        return NULL;
    }

    Factura *facturas = NULL;
    int count = 0;
    char linea[300];

    while (fgets(linea, sizeof(linea), file)) {

        linea[strcspn(linea, "\n")] = 0;

        Factura f;
        int i = 0;
        char temp[100];

        // ID
        leerCampo(linea, &i, temp);
        f.id = atoi(temp);

        // fecha
        leerCampo(linea, &i, f.fecha);

        // cedula
        leerCampo(linea, &i, f.cedula);

        // cliente
        leerCampo(linea, &i, f.cliente);

        // evento
        leerCampo(linea, &i, f.evento);

        // subtotal
        leerCampo(linea, &i, temp);
        f.subtotal = atof(temp);

        // ignorar servicio y total
        leerCampo(linea, &i, temp);
        leerCampo(linea, &i, temp);

        // realloc
        Factura *tempArr = realloc(facturas,
            (count + 1) * sizeof(Factura));

        if (!tempArr) return NULL;

        facturas = tempArr;
        facturas[count++] = f;
    }

    fclose(file);

    *total = count;
    return facturas;
}

/*------------------------------------------------------------
FUNCION: ordenarFacturas
OBJETIVO:
Ordenar facturas por ID ascendente
------------------------------------------------------------*/
void ordenarFacturas(Factura *facturas, int total) {

    for (int i = 0; i < total - 1; i++) {
        for (int j = i + 1; j < total; j++) {

            if (facturas[i].id > facturas[j].id) {

                Factura temp = facturas[i];
                facturas[i] = facturas[j];
                facturas[j] = temp;
            }
        }
    }
}

/*------------------------------------------------------------
FUNCION: mostrarFacturas
OBJETIVO:
Mostrar listado ordenado de facturas
------------------------------------------------------------*/
void mostrarFacturas() {

    int total = 0;
    Factura *facturas = cargarFacturas(&total);

    if (total == 0) {
        printf("\nNo hay facturas registradas.\n");
        return;
    }

    ordenarFacturas(facturas, total);

    printf("\n--- LISTA DE FACTURAS ---\n");
    printf("-----------------------------------------------------------\n");
    printf("ID | Evento | Fecha | Cliente | Subtotal\n");
    printf("-----------------------------------------------------------\n");

    for (int i = 0; i < total; i++) {

        printf("%d | %s | %s | %s | %.2f\n",
               facturas[i].id,
               facturas[i].evento,
               facturas[i].fecha,
               facturas[i].cliente,
               facturas[i].subtotal);
    }

    free(facturas);
}