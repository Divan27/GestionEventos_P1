#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestionSitios.h"

#define MAX 50

// Función para login
int login() {
    char user[MAX], pass[MAX];
    char fileUser[MAX], filePass[MAX];

    FILE *file = fopen("credenciales.txt", "r");

    if (file == NULL) {
        printf("Error al abrir el archivo de credenciales.\n");
        return 0;
    }

    printf("\nLOGIN ADMINISTRATIVO");
    printf("\n------------------------------\n");


    printf("Usuario: ");
    scanf("%s", user);

    printf("Contrasena: ");
    scanf("%s", pass);

    fscanf(file, "%s %s", fileUser, filePass);
    fclose(file);

    if (strcmp(user, fileUser) == 0 && strcmp(pass, filePass) == 0) {
        return 1;
    } else {
        return 0;
    }
}

void gestionEspacios() {
    printf("\n--- Gestion de Espacios ---\n");
}

void gestionEventos() {
    printf("\n--- Gestion de Eventos ---\n");
}

void estadoEvento() {
    printf("\n--- Estado de Evento ---\n");
}

void listaFacturas() {
    printf("\n--- Lista de Facturas ---\n");
}

void estadisticas() {
    printf("\n--- Estadisticas ---\n");
}

void opcionesGenerales() {
    printf("\nOPCIONES GENERALES");
    printf("\n------------------------------\n");
    printf("Aqui se mostraran las opciones generales del sistema.\n");
}


// MENÚ ADMINISTRATIVO (el que ya tenías)
void menuAdministrativo() {
    int opcion;

    do {
        printf("\nMENU ADMINISTRATIVO");
        printf("\n------------------------------\n");
        printf("1. Gestion de Sitios de Eventos\n");
        printf("2. Gestion de Espacios sitio de eventos\n");
        printf("3. Gestion de Eventos\n");
        printf("4. Estado de evento\n");
        printf("5. Lista de facturas\n");
        printf("6. Estadisticas\n");
        printf("7. Volver al menu principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                gestionSitios();
                break;
            case 2:
                gestionEspacios();
                break;
            case 3:
                gestionEventos();
                break;
            case 4:
                estadoEvento();
                break;
            case 5:
                listaFacturas();
                break;
            case 6:
                estadisticas();
                break;
            case 7:
                printf("Regresando al menu principal...\n");
                break;
            default:
                printf("Opcion invalida\n");
        }

    } while(opcion != 7);
}


// MENÚ PRINCIPAL
void menuPrincipal() {
    int opcion;

    do {
        printf("\nSISTEMA DE EVENTOS");
        printf("\n------------------------------\n");
        printf("1. Opciones administrativas\n");
        printf("2. Opciones generales\n");
        printf("3. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                if (login()) {
                    printf("\n*****Acceso concedido*****\n");
                    menuAdministrativo();
                } else {
                    printf("\n*****Acceso denegado*****\n");
                }
                break;

            case 2:
                opcionesGenerales();
                break;

            case 3:
                printf("Saliendo del sistema...\n");
                break;

            default:
                printf("Opcion invalida\n");
        }

    } while(opcion != 3);
}


// Programa principal
int main() {
    menuPrincipal();
    return 0;
}