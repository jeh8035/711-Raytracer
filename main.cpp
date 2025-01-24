#include <cstdint>
#include <string>
#include <format>

#include "tinyga/tinyga.h"

int main() {
    const unsigned int width = 256;
    const unsigned int height = 256;

    TGA image = TGANew(width, height, TGACOLOR(0,0,0));
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            TGASetPixel(&image, x, y, TGACOLOR(128, 128, 128));
        }
    }

    TGADumpFile(&image, "out.tga");
}
