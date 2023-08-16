/*-------------------------------------------------------------------------
 *             Unifal - Universidade Federal de Alfenas
 *                BACHARELADO EM CIENCIA DA COMPUTACAO
 * Trabalho..: Esteganografia
 * Disciplina: Processamento de Imagens
 * Professor.: Luiz Eduardo da Silva
 * Aluno.....: Denis Mendes Coutinho
 * Dara......: 30/06/2023
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "imagelib.h"
#define DEBUG(x) x

void decode(image In)
{
    FILE *file;
    char name[100];
    int fsize;
    unsigned char byte;

    // decode the name
    int i, j, band;
    unsigned char ch;

    band = 0;
    i = 0;
    j = 0;
    do
    {
        ch = 0;
        for (int k = 0; k < 8; k++)
        {
            unsigned char r, g, b;
            unsigned char mask = 0x01, bit = 0;
            r = (In->px[i] >> 16) & 0xFF;
            g = (In->px[i] >> 8) & 0xFF;
            b = In->px[i] & 0xFF;

            if (band == 0)
            {
                bit = r & mask;
            }
            else if (band == 1)
            {
                bit = g & mask;
            }
            else if (band == 2)
            {
                bit = b & mask;
            }

            ch = (ch << 1) | bit;

            mask = mask << 1;
            band = (band + 1) % 3;
            i++;
        }
        name[j++] = ch;
    } while (ch != '\0');
    printf("File name: %s\n", name);

    // decode file size
    fsize = 0;
    for (int k = 0; k < 32; k++)
    {
        unsigned char r, g, b;
        unsigned char mask = 0x01, bit = 0;
        r = (In->px[i] >> 16) & 0xFF;
        g = (In->px[i] >> 8) & 0xFF;
        b = In->px[i] & 0xFF;

        if (band == 0)
        {
            bit = r & mask;
        }
        else if (band == 1)
        {
            bit = g & mask;
        }
        else if (band == 2)
        {
            bit = b & mask;
        }

        fsize = (fsize << 1) | bit;

        mask = mask << 1;
        band = (band + 1) % 3;
        i++;
    }
    printf("File size: %d bytes\n", fsize);

    // decode file
    file = fopen(name, "wb");
    if (!file)
    {
        printf("Cannot create file %s\n", name);
        exit(10);
    }
    for (int k = 0; k < fsize; k++)
    {
        unsigned char ch = 0;
        for (int l = 0; l < 8; l++)
        {
            unsigned char r, g, b;
            unsigned char mask = 0x01, bit = 0;
            r = (In->px[i] >> 16) & 0xFF;
            g = (In->px[i] >> 8) & 0xFF;
            b = In->px[i] & 0xFF;

            if (band == 0)
            {
                bit = r & mask;
            }
            else if (band == 1)
            {
                bit = g & mask;
            }
            else if (band == 2)
            {
                bit = b & mask;
            }

            ch = (ch << 1) | bit;

            mask = mask << 1;
            band = (band + 1) % 3;
            i++;
        }
        fputc(ch, file);
    }
    fclose(file);
}

void msg(char *s)
{
    printf("\nSteganography - decode");
    printf("\n-------------------------------");
    printf("\nUSE.: %s <IMG.PPM>", s);
    printf("\nWhere:\n");
    printf(" <IMG.PPM> Image file in PPM ASCII format\n\n");
    exit(20);
}

int main(int argc, char *argv[])
{
    char name[100];
    image In;
    char *p;
    if (argc < 2)
        msg(argv[0]);
    strcpy(name, argv[1]);
    p = strstr(name, ".ppm");
    if (p)
        *p = 0;
    strcat(name, ".ppm");
    In = img_get(name, COLOR);
    decode(In);
    img_free(In);
    return 0;
}
