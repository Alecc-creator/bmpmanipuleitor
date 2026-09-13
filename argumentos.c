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

int validarArgumentos(int argc, char *argv[], OpcionesPrograma* opciones)
{
    int ret;
    uint8_t encontrado;

    if(argc < 2)
    {
        printf("ERROR: faltan argumentos\n");
        return ERROR_ARGUMENTOS;
    }

    for(int i = 1; i < argc; i++)
    {
        ret = buscarEnFiltros(argv[i], &opciones, &encontrado);

        if(encontrado)
        {
            if(ret != TODO_OK)
                printf("ERROR: filtro invalido: %s\n", argv[i]);


        }


        ///utilidades
        ///archivos
    }

    return TODO_OK;
}

int buscarEnFiltros(const char *argumento, OpcionesPrograma *opciones, uint8_t *encontrado)
{
    size_t largoNombre;
    int valor;
    TipoFiltro tipo;

    *encontrado = 0;

    for(int j = 0; j < CANTIDAD_FILTROS; j++)
    {
        if(!filtrosDisponibles[j].llevaParametro)
        {
            if(strcmp(argumento, filtrosDisponibles[j].nombre) == 0)
            {
                *encontrado = 1;
                tipo = filtrosDisponibles[j].tipo;

                opciones->filtros[tipo].activo = 1;
                opciones->filtros[tipo].valor = 0;

                return TODO_OK;
            }
        }
        else
        {
            largoNombre = strlen(filtrosDisponibles[j].nombre);

            if(strncmp(argumento, filtrosDisponibles[j].nombre, largoNombre) == 0)
            {
                *encontrado = 1;

                tipo = filtrosDisponibles[j].tipo;
                valor = atoi(argumento + largoNombre);

                int ret = validarRango(valor, LIMITE_INFERIOR, LIMITE_SUPERIOR);
                if(ret != TODO_OK)
                    printf("ERROR: parametro fuera de rango en %s\n", argumento);

                opciones->filtros[tipo].activo = 1;
                opciones->filtros[tipo].valor = (uint8_t)valor;

                return TODO_OK;
            }
        }

    }

    return ERROR_ARGUMENTOS;
}



int buscarEnUtilidades(const char *argumento, OpcionesPrograma *opciones, uint8_t *encontrado);
int buscarArchivo(const char *argumento);

int validarRango(int valor, int li, int ls)
{

}
