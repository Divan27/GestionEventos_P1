# GestionEventos_P1
Proyecto programado en C del curso de lenguajes de programación

##  Descripción del problema

El sistema permite la **administración y operación de eventos**, incluyendo:

* Gestión de sitios de eventos
* Asignación de espacios (sectores y asientos)
* Creación de eventos
* Consulta de eventos disponibles
* Compra de boletos
* Generación y consulta de facturas
* Visualización del estado de eventos
* Visualización de estadisticas

El objetivo es simular un sistema real de boletería utilizando:

* **archivos planos (.txt)**
* **memoria dinámica**
* **estructuras y punteros en C**

---

##  Requisitos del sistema

* Compilador C (GCC recomendado)
* Sistema operativo: Windows / Linux / Mac
* Terminal o consola

---


##  Librerías utilizadas

| Librería   | Uso                                           |
| ---------- | --------------------------------------------- |
| `stdio.h`  | Entrada/salida estándar (printf, scanf, FILE) |
| `stdlib.h` | Memoria dinámica (malloc, realloc, free)      |
| `string.h` | Manipulación de cadenas                       |
| `time.h`   | Fecha actual para facturación                 |

---

##  Manejo de archivos

El sistema utiliza archivos `.txt` como base de datos:

### sitios.txt

```
Nombre,Ubicación,Web
```

###  espacios.txt

```
Sitio,Sector,Cantidad,Asientos...
```

Ejemplo:

```
Centro de Eventos,VIP,3,V1,V2,V3
```

---

###  eventos.txt

Formato clave del sistema:

```
Evento,Productora,Fecha,Sitio,Sector:Precio,d:disponibles,v:vendidos
```

Ejemplo:

```
Graduacion,Filma,25/12/26,Centro de Eventos,General:5000.00,d:G1,G2,G3,v:
```

---

###  factura.txt

```
ID,FechaCompra,Cedula,Cliente,Evento,Subtotal,Servicio,Total
```

Ejemplo:

```
1,29/03/26,118010795,Dilan,Party,10000.00,500.00,10500.00
```

---

##  Compilación

Desde la terminal, ubicarse en la carpeta del proyecto y ejecuta:

```bash
gcc programa.c gestionSitios.c gestionEspacios.c gestionEventos.c estadoEventos.c consultaEvento.c listaFacturas.c utils.c -o programa.exe
```

---

## Ejecución

```bash
./programa.exe
```

---

##  Acceso administrativo

El sistema requiere login:

Archivo:

```
credenciales.txt
```

Formato:

```
usuario contraseña
```

---

##  Uso del sistema

###  Menú principal

```
1. Opciones administrativas
2. Opciones generales
3. Salir
```

---

##  Opciones administrativas

### 1. Gestión de sitios

* Cargar sitios desde archivo
* Listar sitios

### 2. Gestión de espacios

* Mostrar espacios
* Agregar espacios
* Resetear espacios

### 3. Gestión de eventos

* Crear eventos
* Asociar sectores y costos

### 4. Estado de eventos

* Ver detalle completo del evento
* Sectores, costos, asientos disponibles/vendidos

### 5. Lista de facturas

* Mostrar facturas generadas
* Ordenadas por ID

---

##  Opciones generales

### 1. Consulta de eventos

* Filtrar eventos por fecha
* Ver sectores y disponibilidad

---

### 2. Compra de boletos

Proceso:

1. Seleccionar evento
2. Elegir sector
3. Seleccionar asiento disponible
4. Ingresar datos del cliente
5. Generar factura

---

##  Lógica del sistema

### Parsing manual

Se implementa lectura manual carácter por carácter:

* `leerCampo()` → separa por coma
* `leerLista()` → maneja `d:` y `v:`

---

###  Memoria dinámica

Uso de:

* `malloc`
* `realloc`
* `free`

Ejemplo:

```c
Sitio **sitios;
sitios = realloc(sitios, n * sizeof(Sitio *));
```

---

### Punteros a estructuras

Uso del operador:

```c
sitios[i]->nombre
```

---

## Ejemplo de flujo completo

1. Cargar sitios
2. Agregar espacios
3. Crear evento
4. Consultar evento
5. Comprar boleto
6. Generar factura
7. Ver estado actualizado

---

##  Consideraciones importantes

* Los archivos deben existir o serán creados automáticamente
* El formato de los `.txt` debe respetarse estrictamente
* No hay base de datos, todo es persistencia en archivos
* El sistema no maneja concurrencia

---

##  Autor

Dilan Zamora Sánchez
Daryll Martinez

---

## Conclusión

Este sistema implementa un modelo completo de gestión de eventos utilizando únicamente C estándar, demostrando:

* Manejo de memoria
* Manejo de punteros
* Diseño modular
* Persistencia en archivos

---

