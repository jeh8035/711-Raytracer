/* tinyga.h
    tiny C library to write raw uncompressed 24bit RGB TGA images. no support for reading, only writing.

    assumes access to the std library, including malloc() and free().

    https://github.com/CPunch/tinyga
*/

#ifndef TINYGA_H
#define TINYGA_H

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* ======================================= [[ TGA API ]] ======================================= */

#pragma pack(push, 1)
typedef struct
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
} TGAColor;
#pragma pack(pop)

typedef struct
{
    TGAColor *data; /* holds the raw image data field to be written */
    uint16_t width;
    uint16_t height;
} TGA;

typedef enum
{
    TGA_OK,
    TGA_ERR_FOPEN,
    TGA_ERR_FWRITE,
} TGAErrorCode;

#define TGACOLOR(_r, _g, _b) ((TGAColor){.b = _b, .g = _g, .r = _r})

const char *TGAError2Str(TGAErrorCode err);

TGA TGANew(int width, int height, TGAColor background);
void TGAClose(TGA *tga);

/* writes TGA data to file, returns TGA_OK if successful */
TGAErrorCode TGADumpFile(TGA *tga, const char *outPath);

void TGASetPixel(TGA *tga, int x, int y, TGAColor color);
void TGAFillColor(TGA *tga, int x, int y, int width, int height, TGAColor color);

#endif