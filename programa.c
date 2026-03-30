#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Inclusión de los módulos del sistema.
    Cada archivo .h contiene la declaración de funciones
    que permiten organizar el programa en partes independientes.
*/
#include "gestionSitios.h"
#include "gestionEspacios.h"
#include "gestionEventos.h"
#include "estadoEventos.h"
#include "consultaEvento.h"
#include "compraBoletos.h"
#include "listaFacturas.h"

#define MAX 50   // tamaño máximo para usuario y contraseña


/*
    FUNCION: login

    Permite validar el acceso al menú administrativo del sistema.
    Lee las credenciales desde el archivo "credenciales.txt"
    y las compara con los datos ingresados por el usuario.

    RETORNA:
    1 -> acceso concedido
    0 -> acceso denegado
*/
int login() {

    // variables para guardar datos ingresados
    char user[MAX], pass[MAX];

    // variables para guardar datos del archivo
    char fileUser[MAX], filePass[MAX];

    // apertura del archivo de credenciales en modo lectura
    FILE *file = fopen("credenciales.txt", "r");

    // validación de apertura del archivo
    if (file == NULL) {
        printf("Error al abrir el archivo de credenciales.\n");
        return 0;
    }

    // interfaz de ingreso de datos
    printf("\nLOGIN ADMINISTRATIVO");
    printf("\n------------------------------\n");

    printf("Usuario: ");
    scanf("%s", user);

    printf("Contrasena: ");
    scanf("%s", pass);

    // lectura de usuario y contraseña almacenados en archivo
    fscanf(file, "%s %s", fileUser, filePass);

    fclose(file); // cerrar archivo

    // comparación de credenciales
    if (strcmp(user, fileUser) == 0 && strcmp(pass, filePass) == 0) {
        return 1; // acceso correcto
    } else {
        return 0; // acceso incorrecto
    }
}


/*
    FUNCION: listaFacturas

    Muestra la lista de facturas generadas en el sistema.
    Actualmente es una función base que se completará posteriormente.
*/
void listaFacturas() {
    mostrarFacturas();
}


/*
    FUNCION: estadisticas

    Mostrará estadísticas relacionadas con los eventos,
    como cantidad de entradas vendidas o ingresos generados.

    Actualmente se encuentra como función base.
*/
void estadisticas() {
    printf("\n--- Estadisticas ---\n");
}


/*
    FUNCION: opcionesGenerales

    Contiene las opciones disponibles para usuarios generales
    (no administradores).

    Ejemplo futuro:
    - ver eventos disponibles
    - comprar entradas
    - consultar precios
*/
void opcionesGenerales() {

    int op;

    do {

        printf("\nOPCIONES GENERALES");
        printf("\n------------------------------\n");

        printf("1. Consulta por evento\n");
        printf("2. Compra de boletos\n");
        printf("3. Salir\n");

        printf("Seleccione una opcion: ");
        scanf("%d", &op);

        switch(op) {

            case 1:
                consultaEvento();
                break;

            case 2:
                compraBoletos();
                break;

            case 3:
                break;

            default:
                printf("Opcion invalida\n");
        }

    } while(op != 3);
}


/*
    FUNCION: menuAdministrativo

    Muestra el menú exclusivo para administradores.

    Desde este menú se puede acceder a:
    1. Gestión de sitios de eventos
    2. Gestión de espacios de los sitios
    3. Gestión de eventos
    4. Estado de eventos
    5. Lista de facturas
    6. Estadísticas
*/
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
                
                estadoEventos();
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


/*
    FUNCION: menuPrincipal

    Es el menú inicial del sistema.

    Permite acceder a:
    1. Opciones administrativas (requiere login)
    2. Opciones generales (usuarios)
    3. Salir del sistema
*/
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

                // validar acceso administrativo
                if (login()) {

                    printf("\n*****Acceso concedido*****\n");

                    
                    menuAdministrativo();

                } else {

                    printf("\n*****Acceso denegado*****\n");
                }

                break;

            case 2:

                // acceso a funcionalidades para usuarios
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


/*
    FUNCION PRINCIPAL

    Punto de inicio del programa.
    Llama al menú principal del sistema.
*/
int main() {

    menuPrincipal();

    return 0;
}
