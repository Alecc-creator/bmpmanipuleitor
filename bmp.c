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
