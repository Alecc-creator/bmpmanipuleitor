#ifndef BMP_H_INCLUDED
#define BMP_H_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include "codigos.h"

#define PROFUNDIDAD             24
#define TAM_FIRMA_BMP           2
#define OFFSET_TAMANIO_ARCHIVO  2
#define OFFSET_INICIO_PIXELES   10
#define OFFSET_ANCHO            18
#define OFFSET_PLANOS           26


typedef struct
{
    uint8_t azul;
    uint8_t verde;
    uint8_t rojo;
}Pixel;

typedef struct
{
    uint32_t tamArchivo;
    uint32_t offsetPixeles;

    uint32_t ancho;
    uint32_t alto;

    uint16_t planos;
    uint16_t bitsPorPixel;

    uint32_t compresion;
    uint32_t tamImagen;
}CabeceraBMP;

int validarArchivoLectura(const char* nomArch);
int validarBMP(const char* nomArch);

int leerCabeceraBMP(FILE* arch, CabeceraBMP* cabecera);

#endif // BMP_H_INCLUDED
