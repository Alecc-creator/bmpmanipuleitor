#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argumentos.h"

static const DefinicionFiltro filtrosDisponibles[] =
{
    {"--negativo", FILTRO_NEGATIVO, 0},
    {"--escala-de-grises", FILTRO_ESCALA_GRISES, 0},
    {"--espejar-horizontal", FILTRO_ESPEJAR_HORIZONTAL, 0},
    {"--espejar-vertical", FILTRO_ESPEJAR_VERTICAL, 0},
    {"--aumentar-contraste=", FILTRO_AUMENTAR_CONTRASTE, 1},
    {"--reducir-contraste=", FILTRO_REDUCIR_CONTRASTE, 1},
    {"--tonalidad-azul=", FILTRO_TONALIDAD_AZUL, 1},
    {"--tonalidad-verde=", FILTRO_TONALIDAD_VERDE, 1},
    {"--tonalidad-roja=", FILTRO_TONALIDAD_ROJA, 1},
    {"--recortar=", FILTRO_RECORTAR, 1},
    {"--achicar=", FILTRO_ACHICAR, 1},
    {"--rotar-derecha", FILTRO_ROTAR_DERECHA, 0},
    {"--rotar-izquierda", FILTRO_ROTAR_IZQUIERDA, 0},
    {"--concatenar-horizontal", FILTRO_CONCATENAR_HORIZONTAL, 0},
    {"--concatenar-vertical", FILTRO_CONCATENAR_VERTICAL, 0},
    {"--comodin", FILTRO_COMODIN, 0}
};

/*
#define CANTIDAD_DEFINICIONES \
    (sizeof(filtrosDisponibles) / sizeof(filtrosDisponibles[0]))
*/

static const DefinicionUtilidad utilidadesDisponibles[] =
{
    {"--info", UTILIDAD_INFO},
    {"--validar", UTILIDAD_VALIDAR},
    {"--verbose", UTILIDAD_VERBOSE},
    {"--help", UTILIDAD_HELP}
};

/*
#define CANTIDAD_DEFINICIONES_UTILIDADES
*/

void inicializarOpciones(OpcionesPrograma *opciones)
{
    for(uint8_t i = 0; i < CANTIDAD_UTILIDADES; i++)
        opciones->utilidades[i] = 0;

    for(uint8_t i = 0; i < CANTIDAD_FILTROS; i++)
    {
        opciones->filtros[i].activo = 0;
        opciones->filtros[i].valor = 0;
    }

    opciones->archivos[0] = NULL;
    opciones->archivos[1] = NULL;

    opciones->cantidadArchivos = 0;
}

int validarArgumentos(int argc, char *argv[], OpcionesPrograma* opciones)
{
    int ret;

    inicializarOpciones(opciones);

    if(argc < 2)
    {
        printf("ERROR: faltan argumentos\n");
        return ERROR_ARGUMENTOS;
    }

    for(int i = 1; i < argc; i++)
    {
        ret = buscarEnFiltros(argv[i], opciones);
        if(ret != TODO_OK)
            ret = buscarEnUtilidades(argv[i], opciones);

        if(ret != TODO_OK)
            ret = buscarArchivo(argv[i], opciones);

        if(ret !=  TODO_OK)
        {
            printf("ERROR: Argumento invalido: %s\n", argv[i]);
            return ERROR_ARGUMENTOS;
        }
    }

    ret = validarCantidadDeArchivos(opciones);
    if(ret != TODO_OK)
        return ret;



    return TODO_OK;
}

int buscarEnFiltros(const char *argumento, OpcionesPrograma *opciones)
{
    size_t largoNombre;
    int valor, ret;
    TipoFiltro tipo;

    for(int j = 0; j < CANTIDAD_FILTROS; j++)
    {
        if(!filtrosDisponibles[j].llevaParametro)
        {
            if(strcmp(argumento, filtrosDisponibles[j].nombre) == 0)
            {
                tipo = filtrosDisponibles[j].tipo;

                opciones->filtros[tipo].activo = 1;/// tipo indica que filtro activar
                opciones->filtros[tipo].valor = 0;

                return TODO_OK;
            }
        }
        else
        {
            largoNombre = strlen(filtrosDisponibles[j].nombre);

            if(strncmp(argumento, filtrosDisponibles[j].nombre, largoNombre) == 0)
            {
                tipo = filtrosDisponibles[j].tipo;
                ret = atoi_alu(argumento + largoNombre, &valor);
                if(ret != TODO_OK)
                    return ret;

                ret = validarRango(valor, LIMITE_INFERIOR, LIMITE_SUPERIOR);
                if(ret != TODO_OK)
                    return ret;

                opciones->filtros[tipo].activo = 1;
                opciones->filtros[tipo].valor = (uint8_t)valor;

                return TODO_OK;
            }
        }

    }

    return ERROR_ARGUMENTOS;
}

int buscarEnUtilidades(const char *argumento, OpcionesPrograma *opciones)
{
    TipoUtilidad tipo;

    for(int i = 0; i < CANTIDAD_UTILIDADES; i++)
    {
        if(strcmp(argumento, utilidadesDisponibles[i].nombre) == 0)
        {
            tipo = utilidadesDisponibles[i].tipo;
            opciones->utilidades[tipo] = 1;

            return TODO_OK;
        }
    }

    return ERROR_ARGUMENTOS;
}

int buscarArchivo(const char *argumento, OpcionesPrograma* opciones)
{
    ///VERIFICAMOS LA CANTIDAD DE ARCHIVOS Y EL FORMATO. LA EXISTENCIA REAL DEL ARCHIVO Y QUE SEA BMP LO MIRAMOS EN OTRA FUNCION
    if(argumento[0] == '-')
    {
        printf("ERROR: Formato archivo invalido.\n");
        return ERROR_ARGUMENTOS;
    }

    if(opciones->cantidadArchivos >= 2)
    {
        printf("ERROR: Supera la cantidad de archivos permitido.\n");
        return ERROR_ARGUMENTOS;
    }

    opciones->archivos[opciones->cantidadArchivos] = argumento;
    opciones->cantidadArchivos++;

    return TODO_OK;
}

int validarCantidadDeArchivos(const OpcionesPrograma* opciones)
{
    if(opciones->utilidades[UTILIDAD_HELP])
        return TODO_OK;

    uint8_t usaConcatenacion = 0;

    if(opciones->filtros[FILTRO_CONCATENAR_HORIZONTAL].activo == 1 ||
       opciones->filtros[FILTRO_CONCATENAR_VERTICAL].activo == 1)
       usaConcatenacion = 1;

    if(usaConcatenacion)
    {
        if(opciones->cantidadArchivos != 2)
        {
            printf("ERROR: la cantidad de archivos debe ser 2.\n");
            return ERROR_ARGUMENTOS;
        }
    }
    else
    {
        if(opciones->cantidadArchivos != 1)
        {
            printf("ERROR: la cantidad de archivos debe ser 1.\n");
            return ERROR_ARGUMENTOS;
        }
    }

    return TODO_OK;
}

int validarConcatenaciones(const OpcionesPrograma* opciones)
{
    if(opciones->filtros[FILTRO_CONCATENAR_HORIZONTAL]. activo &&
       opciones->filtros[FILTRO_CONCATENAR_VERTICAL].activo)
    {
        printf("ERROR: solo se usa concatenar HORIZONTAL o VERTICAL. No ambos a la misma vez.\n");
        return ERROR_ARGUMENTOS;
    }

    return TODO_OK;
}

int validarRango(int valor, int li, int ls)
{
    if(valor < li || valor > ls)
    {
        printf("Valor fuera de rango.\n");
        return ERROR_ARGUMENTOS;
    }


    return TODO_OK;
}

int atoi_alu(const char* cadena, int* valor)
{
    int numero = 0;

    if(cadena == NULL || *cadena == '\0')
    {
        printf("Valor vacio.\n");
        return ERROR_ARGUMENTOS;
    }

    while(*cadena != '\0')
    {
        if(*cadena < '0' || *cadena > '9')
        {
            printf("Valor no numerico.\n");
            return ERROR_ARGUMENTOS;
        }

        numero = numero * 10 + (*cadena - '0');
        cadena++;
    }

    *valor = numero;

    return TODO_OK;
}
