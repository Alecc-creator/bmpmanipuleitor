#include "utilidades.h"

#include "stdio.h"

void mostrarHelp()
{
    printf("BMPMANIPULEITOR - Manipulador de imágenes BMP 24 bits\n"
            "GRUPO: RECURSIVIDAD\n"
            "Integrantes:\n"
            "1. 41202088 - CUEVAS CARDOZO, Alejandro\n"
            "2. 40239700 - DEDO, Juan Pablo\n"
            "3. 45148037 - VELAZ, Ezequiel\n"
            "Uso: bmpmanipuleitor.exe [OPCIONES]\n"


            "EJEMPLOS:\n"
            "bmpmanipuleitor.exe --negativo foto.bmp\n"
            "bmpmanipuleitor.exe --info imagen.bmp --validar\n"
            "bmpmanipuleitor.exe foto.bmp --verbose --escala-de-grises --aumentar-contraste=25\n"

            "FILTROS\n"
            "--comodin <Explicacion de esta funcionalidad personalizada>\n");
}
