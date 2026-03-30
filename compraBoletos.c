#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

/*------------------------------------------------------------*/
int generarIDFactura() {
    static int id = 1;
    return id++;
}

/*------------------------------------------------------------*/
void obtenerFechaActual(char *fecha) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    sprintf(fecha, "%02d/%02d/%02d",
            tm->tm_mday,
            tm->tm_mon + 1,
            (tm->tm_year + 1900) % 100);
}

/*------------------------------------------------------------*/
/* Eliminar asiento de lista */
void eliminarAsiento(char *lista, char *asiento) {

    char copia[200] = "";
    char temp[20];
    int i = 0, j = 0;

    while (lista[i] != '\0') {

        j = 0;

        while (lista[i] != ',' && lista[i] != '\0') {
            temp[j++] = lista[i++];
        }

        temp[j] = '\0';

        if (strcmp(temp, asiento) != 0) {
            if (strlen(copia) > 0) strcat(copia, ",");
            strcat(copia, temp);
        }

        if (lista[i] == ',') i++;
    }

    strcpy(lista, copia);
}

/*------------------------------------------------------------*/
/* Agregar asiento a lista */
void agregarAsiento(char *lista, char *asiento) {

    if (strlen(lista) > 0) strcat(lista, ",");
    strcat(lista, asiento);
}

/*------------------------------------------------------------*/
void compraBoletos() {

    FILE *file = fopen("eventos.txt", "r");
    if (!file) {
        printf("No hay eventos disponibles.\n");
        return;
    }

    char lineas[100][500];
    int total = 0;

    while (fgets(lineas[total], 500, file)) {
        lineas[total][strcspn(lineas[total], "\n")] = 0;
        total++;
    }

    fclose(file);

    // 🔹 Mostrar eventos
    printf("\n--- EVENTOS ---\n");
    for (int k = 0; k < total; k++) {
        char nombre[100];
        int idx = 0;
        leerCampo(lineas[k], &idx, nombre);
        printf("%d - %s\n", k + 1, nombre);
    }

    int op;
    printf("Seleccione evento: ");
    scanf("%d", &op);

    if (op < 1 || op > total) {
        printf("Opcion invalida.\n");
        return;
    }

    char nuevaLinea[500] = "";
    char *linea = lineas[op - 1];

    char nombre[100], productora[100], fecha[20], sitio[100];
    int i = 0;

    leerCampo(linea, &i, nombre);
    leerCampo(linea, &i, productora);
    leerCampo(linea, &i, fecha);
    leerCampo(linea, &i, sitio);

    sprintf(nuevaLinea, "%s,%s,%s,%s", nombre, productora, fecha, sitio);

    float subtotal = 0;

    // recorrer sectores
    while (linea[i] != '\0') {

        char campo[200];
        leerCampo(linea, &i, campo);

        if (strchr(campo, ':') &&
            !(campo[0] == 'd' && campo[1] == ':') &&
            !(campo[0] == 'v' && campo[1] == ':')) {

            char sector[100];
            float costo;

            sscanf(campo, "%[^:]:%f", sector, &costo);

            char disponibles[200] = "";
            char vendidos[200] = "";

            // leer d:
            if (linea[i] == 'd' && linea[i + 1] == ':') {
                i += 2;
                leerLista(linea, &i, disponibles);
            }

            // leer v:
            if (linea[i] == 'v' && linea[i + 1] == ':') {
                i += 2;
                leerLista(linea, &i, vendidos);
            }

            printf("\nSector: %s (%.2f)\n", sector, costo);
            printf("Disponibles: %s\n",
                   strlen(disponibles) ? disponibles : "Ninguno");

            char asiento[20];
            printf("Seleccione asiento (0 para omitir): ");
            scanf("%s", asiento);

            if (strcmp(asiento, "0") != 0) {

                if (strstr(disponibles, asiento)) {

                    subtotal += costo;

                    // 🔥 MOVER ASIENTO
                    eliminarAsiento(disponibles, asiento);
                    agregarAsiento(vendidos, asiento);

                } else {
                    printf("Asiento no disponible.\n");
                }
            }

            // reconstruir sector en nueva línea
            char buffer[300];
            sprintf(buffer, ",%s:%.2f,d:%s,v:%s",
                    sector, costo, disponibles, vendidos);

            strcat(nuevaLinea, buffer);
        }
    }

    if (subtotal == 0) {
        printf("No se realizo compra.\n");
        return;
    }

    char cedula[20], nombreCliente[100];

    printf("Cedula: ");
    scanf("%s", cedula);

    printf("Nombre: ");
    scanf("%s", nombreCliente);

    float servicio = subtotal * 0.05;
    float totalPagar = subtotal + servicio;

    int idFactura = generarIDFactura();

    char fechaCompra[20];
    obtenerFechaActual(fechaCompra);

    // FACTURA
    printf("\n--- FACTURA ---\n");
    printf("ID: %d\n", idFactura);
    printf("Fecha: %s\n", fechaCompra);
    printf("Cliente: %s\n", nombreCliente);
    printf("Cedula: %s\n", cedula);
    printf("Evento: %s\n", nombre);
    printf("Subtotal: %.2f\n", subtotal);
    printf("Servicio: %.2f\n", servicio);
    printf("TOTAL: %.2f\n", totalPagar);

    // guardar factura
    FILE *fact = fopen("factura.txt", "a");
    if (fact) {
        fprintf(fact,
            "%d,%s,%s,%s,%s,%.2f,%.2f,%.2f\n",
            idFactura, fechaCompra, cedula, nombreCliente,
            nombre, subtotal, servicio, totalPagar);
        fclose(fact);
    }

    // ACTUALIZAR ARCHIVO eventos.txt
    FILE *rewrite = fopen("eventos.txt", "w");

    for (int k = 0; k < total; k++) {
        if (k == op - 1)
            fprintf(rewrite, "%s\n", nuevaLinea);
        else
            fprintf(rewrite, "%s\n", lineas[k]);
    }

    fclose(rewrite);

    printf("\n***** Compra realizada y evento actualizado *****\n");
}