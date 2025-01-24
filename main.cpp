#include "Libraries/tinyga/tinyga.h"
#include "Libraries/matrix/matrix.h"

#include "Primitives/shape.h"
#include "Primitives/camera.h"

int main() {
    const unsigned int width = 256;
    const unsigned int height = 256;

    // Create image
    TGA image = TGANew(width, height, TGACOLOR(0,0,0));
    
    // Define objects
    Primitives::Camera camera = Primitives::Camera(Primitives::Point({1.9, -3.0, 1.1}), Primitives::Direction({1.0, 0.0, 0.0}));

    Primitives::Sphere sphere1 = Primitives::Sphere(0.5, Primitives::Point({1.8f, 1.5f, 1.2f}));
    Primitives::Sphere sphere2 = Primitives::Sphere(0.5, Primitives::Point({2.6f, 3.0f, 0.7f}));

    Primitives::Shape* shapes[] = { 
        &sphere1,
        &sphere2,
    };

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            TGASetPixel(&image, x, y, TGACOLOR(128, 128, 128));
        }
    }

    TGADumpFile(&image, "out.tga");
}
