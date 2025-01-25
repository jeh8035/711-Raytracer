#include "Libraries/tinyga/tinyga.h"
#include "Libraries/matrix/matrix.h"

#include "Primitives/shape.h"
#include "Primitives/camera.h"
#include "Primitives/filmplane.h"

int main() {
    const unsigned int width = 256;
    const unsigned int height = 256;
    constexpr float aspect_ratio = width/height;

    // Create image
    TGA image = TGANew(width, height, TGACOLOR(0,0,0));
    
    // Define objects
    Primitives::Camera camera = Primitives::Camera(Primitives::Point({1.9, -3.0, 1.1}), Primitives::Direction({1.0, 0.0, 0.0}));
    
    // Film plane in camera space
    Primitives::FilmPlane filmplane = Primitives::FilmPlane(
        Primitives::Point({2.0, 0.0, 0.0}),
        2.0,
        2.0 * aspect_ratio
    );

    Primitives::Sphere sphere1 = Primitives::Sphere(0.5, Primitives::Point({1.8f, 1.5f, 1.2f}));
    Primitives::Sphere sphere2 = Primitives::Sphere(0.5, Primitives::Point({2.6f, 3.0f, 0.7f}));

    Primitives::Shape* shapes[] = { 
        &sphere1,
        &sphere2,
    };

    // Transform objects to camera space
    for (auto shape : shapes) {
        shape->Translate(-camera.GetPosition());
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            TGAColor color = TGACOLOR(0, 0, 0);

            // Cast ray
            Primitives::Direction dir = Primitives::Direction();
            float pixelWidth = filmplane.GetWidth() / width;
            float pixelHeight = filmplane.GetHeight() / height;

            dir = Primitives::Direction({
                filmplane.GetPosition().x(),
                // Pixel offset + offset to middle of pixel - offset to center plane
                (pixelWidth * x) + (pixelWidth / 2) - (pixelWidth * width / 2),
                (pixelHeight * y) + (pixelWidth / 2) - (pixelHeight * height / 2)
            }).normalize();

            Primitives::Ray ray = Primitives::Ray(
                Primitives::Point({0, 0, 0}),
                dir
            );

            // Set pixel color
            TGASetPixel(&image, x, y, color);
        }
    }

    TGADumpFile(&image, "out.tga");
}
