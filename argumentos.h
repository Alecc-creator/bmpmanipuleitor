#ifndef ARGUMENTOS_H_INCLUDED
#define ARGUMENTOS_H_INCLUDED

#include "codigos.h"
#include <stdint.h>

#define LIMITE_INFERIOR 1
#define LIMITE_SUPERIOR 100

typedef enum
{
    FILTRO_NEGATIVO,
    FILTRO_ESCALA_GRISES,
    FILTRO_ESPEJAR_HORIZONTAL,
    FILTRO_ESPEJAR_VERTICAL,
    FILTRO_AUMENTAR_CONTRASTE,
    FILTRO_REDUCIR_CONTRASTE,
    FILTRO_TONALIDAD_AZUL,
    FILTRO_TONALIDAD_VERDE,
    FILTRO_TONALIDAD_ROJA,
    FILTRO_RECORTAR,
    FILTRO_ACHICAR,
    FILTRO_ROTAR_DERECHA,
    FILTRO_ROTAR_IZQUIERDA,
    FILTRO_CONCATENAR_HORIZONTAL,
    FILTRO_CONCATENAR_VERTICAL,
    FILTRO_COMODIN,
    CANTIDAD_FILTROS
}TipoFiltro;

typedef struct
{
    const char *nombre;
    TipoFiltro tipo;
    uint8_t llevaParametro;
}DefinicionFiltro;

typedef struct
{
    uint8_t activo;
    uint8_t valor;
}FiltroConfig;

typedef enum
{
    UTILIDAD_INFO,
    UTILIDAD_VALIDAR,
    UTILIDAD_VERBOSE,
    UTILIDAD_HELP,
    CANTIDAD_UTILIDADES
}TipoUtilidad;

typedef struct
{
    const char *nombre;
    TipoUtilidad tipo;
}DefinicionUtilidad;

typedef struct
{
    uint8_t utilidades[CANTIDAD_UTILIDADES];

    FiltroConfig filtros[CANTIDAD_FILTROS];

    const char *archivos[2];
    uint8_t cantidadArchivos;
} OpcionesPrograma;

void inicializarOpciones(OpcionesPrograma *opciones);

int validarArgumentos(int argc, char* argv[], OpcionesPrograma* opciones);
int buscarEnFiltros(const char *argumento, OpcionesPrograma* opciones);
int buscarEnUtilidades(const char *argumento, OpcionesPrograma* opciones);
int buscarArchivo(const char *argumento, OpcionesPrograma* opciones);
int validarCantidadDeArchivos(const OpcionesPrograma* opciones);
int validarConcatenaciones(const OpcionesPrograma* opciones);
int validarRango(int valor, int li, int ls);

int atoi_alu(const char* cadena, int* valor);

#endif // ARGUMENTOS_H_INCLUDED
