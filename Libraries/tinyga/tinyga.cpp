// Taken from https://github.com/CPunch/tinyga

#include "tinyga.h"

#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_TGA_COLOR_BITLENGTH 24

/* ================================== [[ Helper Functions ]] =================================== */

#pragma pack(push, 1)
typedef struct
{
    uint8_t IIDLength; /* image id length */
    uint8_t colorMapID;
    uint8_t imageType;
    uint16_t colorMapEntry;
    uint16_t colorMapSize;
    uint8_t colorMapBitLength;
    uint16_t xOrigin;
    uint16_t yOrigin;
    uint16_t width;
    uint16_t height;
    uint8_t pixelBitLength; /* we only support 24 bit color rn */
    uint8_t DescFlag;
} TGAHeader;
#pragma pack(pop)

/* ======================================= [[ TGA API ]] ======================================= */

const char *TGAError2Str(TGAErrorCode err)
{
    switch (err) {
    case TGA_OK:
        return "TGA_OK";
    case TGA_ERR_FOPEN:
        return "TGA_ERR_FOPEN";
    case TGA_ERR_FWRITE:
        return "TGA_ERR_FWRITE";
    default:
        return "unkwn";
    }
}

TGA TGANew(int width, int height, TGAColor background)
{
    TGA tga = {0};

    /* could be a large malloc! TODO: mmap file? */
    tga.data = (TGAColor *)malloc(sizeof(TGAColor) * width * height);
    tga.width = width;
    tga.height = height;

    /* write background */
    TGAFillColor(&tga, 0, 0, width, height, background);
    return tga;
}

void TGAClose(TGA *tga)
{
    free(tga->data);
}

/* writes TGA data to file, returns TGA_OK if successful */
TGAErrorCode TGADumpFile(TGA *tga, const char *outPath)
{
    TGAHeader header;
    FILE *out;
    size_t sz = sizeof(TGAColor) * tga->width * tga->height;
    TGAErrorCode err = TGA_OK;

    /* open file */
    if ((out = fopen(outPath, "wb")) == NULL)
        return TGA_ERR_FOPEN;

    /* zero set header (most values are fine being 0) */
    memset(&header, 0, sizeof(TGAHeader));
    header.imageType = 0x02; /* uncompressed RGB data */
    header.width = tga->width;
    header.height = tga->height;
    header.pixelBitLength = 24; /* RGB, 3 bytes per pixel */

    /* write TGA header */
    if (fwrite((void *)&header, sizeof(TGAHeader), 1, out) != 1) {
        err = TGA_ERR_FWRITE;
        goto _TGADumpFileExit;
    }

    /* write image data */
    if (fwrite((void *)tga->data, sz, 1, out) != 1) {
        err = TGA_ERR_FWRITE;
        goto _TGADumpFileExit;
    }

_TGADumpFileExit:
    fclose(out);
    return err;
}

void TGASetPixel(TGA *tga, int x, int y, TGAColor color)
{
    size_t i = (y * tga->width) + x;

    /* sanity check */
    if (i >= (tga->width * tga->height))
        return;

    tga->data[i] = color;
}

void TGAFillColor(TGA *tga, int startX, int startY, int width, int height, TGAColor color)
{
    int x, y, endX = startX + width, endY = startY + height;

    /* fill image */
    for (x = startX; x < endX; x++) {
        for (y = startY; y < endY; y++) {
            TGASetPixel(tga, x, y, color);
        }
    }
}