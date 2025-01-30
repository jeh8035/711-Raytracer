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
    Primitives::Camera camera = Primitives::Camera(Primitives::Point({1.9, 1.1, -3.0}), Primitives::Direction({1.0, 0.0, 0.0}));
    
    // Film plane in camera space
    Primitives::FilmPlane filmplane = Primitives::FilmPlane(
        0.2,
        100.0,
        100.0 * aspect_ratio
    );

    Primitives::Sphere sphere1 = Primitives::Sphere(0.5, Primitives::Point({1.8f, 1.2f, 1.5f}));
    Primitives::Sphere sphere2 = Primitives::Sphere(0.5, Primitives::Point({2.6f, 0.7f, 3.0f}));

    Primitives::Shape* shapes[] = { 
        &sphere1,
        &sphere2,
    };

    // Transform objects to camera space
    for (auto shape : shapes) {
        shape->Translate(-camera.GetPosition());
    }

    for (int y = -height/2; y < height/2; y++) {
        for (int x = -width/2; x < width/2; x++) {
            TGAColor color = TGACOLOR(0, 0, 0);

            // Cast ray
            Primitives::Direction dir = Primitives::Direction();
            float pixelWidth = filmplane.GetWidth() / width;
            float pixelHeight = filmplane.GetHeight() / height;

            // TODO: change this based on camera rotation
            dir = Primitives::Direction({
                // Pixel offset + offset to middle of pixel
                (pixelWidth * x) + (pixelWidth / 2),
                (pixelHeight * y) + (pixelHeight / 2),
                filmplane.GetDist(),
            }).normalize();

            Primitives::Ray ray = Primitives::Ray(
                Primitives::Point({0, 0, 0}),
                dir
            );

            for (Primitives::Shape* shape : shapes) {
                if (shape->Intersect(ray).hit) {
                    color.b = 255;
                }
            }

            // Set pixel color
            TGASetPixel(&image, x, y, color);
        }
    }

    TGADumpFile(&image, "out.tga");
}
