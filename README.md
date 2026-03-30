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
gcc programa.c gestionSitios.c gestionEspacios.c gestionEventos.c estadoEventos.c consultaEvento.c utils.c compraBoletos.c listaFacturas.c -o programa.exe
```

---

## Ejecución

```bash
.\programa.exe
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
## Análisis de resultados del sistema

En general, el sistema desarrollado cumple con la mayoría de los requerimientos planteados, especialmente en el módulo de **Opciones Administrativas**, donde se logró implementar correctamente:

* Gestión de Sitios de Eventos
* Gestión de espacios de sitios
* Gestión de Eventos
* Estado de evento
* Lista de facturas

La única funcionalidad que no fue desarrollada corresponde a **Estadísticas**, quedando como mejora futura.

---

###  Evaluación funcional

Las funcionalidades implementadas permiten:

* Cargar, almacenar y mostrar información desde archivos `.txt`
* Gestionar eventos con sectores y asientos
* Realizar compras y actualizar disponibilidad en tiempo real
* Generar y listar facturas correctamente

Esto demuestra que la lógica principal del sistema está **operativa y coherente** con el flujo esperado.

---

### Limitaciones detectadas

Durante la ejecución del programa se identificaron algunos problemas principalmente relacionados con validaciones:

*  Falta de validación de fechas (formatos incorrectos pueden afectar comparaciones)
*  No se controla correctamente la entrada de datos no numéricos en menús (`scanf`)
*  Ingreso de letras en opciones numéricas puede generar comportamientos inesperados o bucles
*  Algunas validaciones de datos de entrada (como campos vacíos o formatos incorrectos) no están completamente controladas

---

###  Opciones generales

El módulo de **Opciones Generales** se encuentra prácticamente completo:

* Consulta de eventos 
* Compra de boletos 

Sin embargo, al igual que en el módulo administrativo, presenta debilidades en:

* Validación de entradas del usuario
* Control de errores en datos ingresados

Lo que puede afectar el flujo del programa si el usuario ingresa información no esperada.

---
##  Justificación de toma de decisiones

### a. Uso de punteros en el sistema

En el proyecto se usaron punteros principalmente para poder trabajar con **memoria dinámica** y estructuras que crecen según la información de los archivos.

Por ejemplo, al cargar sitios o eventos no se sabe cuántos habrá, entonces se usa `realloc` para ir ampliando el arreglo en tiempo de ejecución. Esto hace el programa más flexible.

También se usan punteros para **evitar copias innecesarias** de datos grandes (como structs), accediendo directamente con la flechita `->`, lo cual mejora el rendimiento.

Además, los punteros permiten trabajar **por referencia**, lo que significa que una función puede modificar directamente una variable externa.

* Paso por valor: no cambia el dato original
* Paso por referencia: sí modifica el dato original

Esto se usa bastante en funciones que procesan información del archivo.

---

### b. Modularidad y ocultamiento de información

El sistema está dividido en varios archivos (`.c` y `.h`), donde cada uno tiene una función específica, como manejar sitios, eventos o facturas.

Esto permite:

* Mantener el código ordenado
* Facilitar el mantenimiento
* Reutilizar funciones

Los archivos `.h` solo muestran las funciones necesarias, mientras que la lógica interna queda en los `.c`, lo que ayuda a **ocultar detalles de implementación**.

También se usan funciones generales en `utils.c` para evitar repetir código, como leer datos del archivo o procesar fechas.

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

