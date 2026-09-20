#ifndef BMP_H_INCLUDED
#define BMP_H_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include "codigos.h"
#include "matriz.h"

#define PROFUNDIDAD             24
#define TAM_FIRMA_BMP           2
#define OFFSET_TAMANIO_ARCHIVO  2
#define OFFSET_INICIO_PIXELES   10
#define OFFSET_ANCHO            18
#define OFFSET_ALTO             22
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

typedef struct
{
    CabeceraBMP cabecera;
    Pixel** imagen;
}ImagenBMP;

int validarArchivoLectura(const char* nomArch);
int validarBMP(const char* nomArch);
int leerCabeceraBMP(FILE* arch, CabeceraBMP* cabecera);

int crearImagenBMP(ImagenBMP* bmp);
void destruirImagenBMP(ImagenBMP* bmp);
int obtenerAltoReal(int32_t alto);
uint8_t calcularPadding(int32_t ancho);
int leerPixelesBMP(FILE* archivo, ImagenBMP* bmp);
int cargarBMP(const char* arch, ImagenBMP* bmp);

#endif // BMP_H_INCLUDED
