#include "bmp.h"

int validarArchivoLectura(const char* nomArch)
{
    FILE* arch;

    arch = fopen(nomArch, "rb");
    if(!arch)
    {
        printf("ERROR: No se pudo abrir el archivo %s.\n", nomArch);
        return ERROR_ARCHIVO;
    }

    fclose(arch);

    return TODO_OK;
}

int validarBMP(const char* nomArch)
{
    FILE* arch;
    CabeceraBMP cabecera;
    int ret;

    arch = fopen(nomArch, "rb");
    if(!arch)
    {
        printf("ERROR: No se pudo abrir el archivo %s.\n", nomArch);
        return ERROR_ARCHIVO;
    }

    ret = leerCabeceraBMP(arch, &cabecera);
    if(ret != TODO_OK)
    {
        fclose(arch);
        return ret;
    }

    if(cabecera.ancho < 1 || cabecera.alto == 0)
    {
        printf("ERROR: Dimensiones invalidas.\n");
        fclose(arch);
        return ERROR_BMP_INVALIDO;
    }

    if(cabecera.planos != 1)
    {
        printf("ERROR: Plano invalido.\n");
        fclose(arch);
        return ERROR_BMP_INVALIDO;
    }

    if(cabecera.bitsPorPixel != PROFUNDIDAD)
    {
        printf("ERROR: La profundidad debe ser de %d bits.\n", PROFUNDIDAD);
        fclose(arch);
        return ERROR_BMP_INVALIDO;
    }

    if(cabecera.compresion != 0)
    {
        printf("ERROR: El archivo NO debe ser comprimido.\n");
        fclose(arch);
        return ERROR_BMP_INVALIDO;
    }

    fclose(arch);

    return TODO_OK;
}

int leerCabeceraBMP(FILE* arch, CabeceraBMP* cabecera)
{
    unsigned char firma[TAM_FIRMA_BMP];

    if(fseek(arch, 0, SEEK_SET) != 0)
        return ERROR_ARCHIVO;

    if(fread(firma, sizeof(unsigned char), TAM_FIRMA_BMP, arch) != 2)
        return ERROR_BMP_INVALIDO;

    if(firma[0] != 'B' || firma[1] != 'M')
    {
        printf("ERROR: el archivo no es BMP.\n");
        return ERROR_BMP_INVALIDO;
    }

    if(fseek(arch, OFFSET_TAMANIO_ARCHIVO, SEEK_SET) != 0)
        return ERROR_ARCHIVO;

    if(fread(&cabecera->tamArchivo, sizeof(uint32_t), 1, arch) != 1)
        return ERROR_BMP_INVALIDO;

    if(fseek(arch, OFFSET_INICIO_PIXELES, SEEK_SET) != 0)
        return ERROR_ARCHIVO;

    if(fread(&cabecera->offsetPixeles, sizeof(uint32_t), 1, arch) != 1)
        return ERROR_BMP_INVALIDO;

    if(fseek(arch, OFFSET_ANCHO, SEEK_SET) != 0)
        return ERROR_ARCHIVO;

    if(fread(&cabecera->ancho, sizeof(int32_t), 1, arch) != 1)
        return ERROR_BMP_INVALIDO;

    if(fseek(arch, OFFSET_ALTO, SEEK_SET) != 0)
        return ERROR_ARCHIVO;

    if(fread(&cabecera->alto, sizeof(int32_t), 1, arch) != 1)
        return ERROR_BMP_INVALIDO;

    if(fseek(arch, OFFSET_PLANOS, SEEK_SET) != 0)
        return ERROR_ARCHIVO;

    if(fread(&cabecera->planos, sizeof(uint16_t), 1, arch) != 1)
        return ERROR_BMP_INVALIDO;

    if(fread(&cabecera->bitsPorPixel, sizeof(uint16_t), 1, arch) != 1)
        return ERROR_BMP_INVALIDO;

    if(fread(&cabecera->compresion, sizeof(uint32_t), 1, arch) != 1)
        return ERROR_BMP_INVALIDO;

    if(fread(&cabecera->tamImagen, sizeof(uint32_t), 1, arch) != 1)
        return ERROR_BMP_INVALIDO;

    return TODO_OK;
}

int crearImagenBMP(ImagenBMP* bmp)
{
    if(!bmp)
        return ERROR_MEMORIA;

    int filas = obtenerAltoReal(bmp->cabecera.alto); ///ALTO PUEDE ESTAR EN NEGATIVO, POR ESO SE BUSCA EL MODULO

    bmp->imagen = (Pixel**)crearMatriz(filas, bmp->cabecera.ancho, sizeof(Pixel));
    if(!bmp->imagen)
        return ERROR_MEMORIA;

    return TODO_OK;
}

void destruirImagenBMP(ImagenBMP* bmp)
{
    if(!bmp || !bmp->imagen)
        return;

    int filas = obtenerAltoReal(bmp->cabecera.alto);

    destruirMatriz((void**)bmp->imagen, filas);

    bmp->imagen = NULL;
}

int obtenerAltoReal(int32_t alto)
{
    return alto < 0 ? -alto : alto;
}

uint8_t calcularPadding(int32_t ancho) ///PIXEL TIENE 3 bytes
{
    return (4 - (ancho * sizeof(Pixel)) % 4) % 4;
}

int leerPixelesBMP(FILE* archivo, ImagenBMP* bmp)
{
    int32_t filas = obtenerAltoReal(bmp->cabecera.alto);
    int32_t columnas = bmp->cabecera.ancho;
    uint32_t padding = calcularPadding(columnas);

    if(fseek(archivo, bmp->cabecera.offsetPixeles, SEEK_SET) != 0)
        return ERROR_ARCHIVO;

    for(int32_t i = 0; i < filas; i++)
    {
        int32_t filaDestino;

        if(bmp->cabecera.alto > 0)
            filaDestino = filas - 1 - i;
        else
            filaDestino = i;

        for(int32_t j = 0; j < columnas; j++)
        {
            if(fread(&bmp->imagen[filaDestino][j], sizeof(Pixel), 1, archivo) != 1)
                return ERROR_ARCHIVO;
        }

        if(fseek(archivo, padding, SEEK_CUR) != 0)
            return ERROR_ARCHIVO;

        ///ejemplo con ancho = 3 -> 9 b de pixeles + 3 b de padding = 12 b
        ///[pixel][pixel][pixel][padding padding padding]
        ///[pixel][pixel][pixel][padding padding padding]
        ///[pixel][pixel][pixel][padding padding padding]
    }

    return TODO_OK;
}

int cargarBMP(const char* nomArch, ImagenBMP* bmp)
{
    FILE* arch;
    int ret;

    if(!nomArch || !bmp)
        return ERROR_ARGUMENTOS;

    arch = fopen(nomArch, "rb");
    if(!arch)
    {
        printf("ERROR: No se pudo abrir el archivo %s.\n", nomArch);
        return ERROR_ARCHIVO;
    }


    ret = leerCabeceraBMP(arch, &bmp->cabecera);
    if(ret != TODO_OK)
    {
        fclose(arch);
        return ret;
    }

    ret = crearImagenBMP(bmp);
    if(ret != TODO_OK)
    {
        fclose(arch);
        return ret;
    }

    ret = leerPixelesBMP(arch, bmp);

    fclose(arch);

    if(ret != TODO_OK)
    {
        destruirImagenBMP(bmp);
        return ret;
    }

    return TODO_OK;
}
