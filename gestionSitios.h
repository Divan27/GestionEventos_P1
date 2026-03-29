#ifndef GESTION_SITIOS_H
#define GESTION_SITIOS_H

typedef struct {
    char nombre[100];
    char ubicacion[100];
    char web[100];
} Sitio;

// variables globales con extern
extern Sitio **sitios;
extern int cantidadSitios;

// funciones
void gestionSitios();
void cargarSitiosLote();
void listarSitios();

#endif
