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
    
    // Create camera
    Primitives::Camera camera = Primitives::Camera(Primitives::Point({1.1, 1.9, -3.0}), Primitives::Direction({0.0, 0.0, 1.0}));
    
    // Create film plane
    Primitives::FilmPlane filmplane = Primitives::FilmPlane(
        1.0,
        0.7,
        0.7 * aspect_ratio
    );

    // Define objects
    Primitives::Sphere sphere1 = Primitives::Sphere(0.5, Primitives::Point({1.4f, 1.8f, 3.0f}));
    Primitives::Sphere sphere2 = Primitives::Sphere(0.5, Primitives::Point({1.0f, 2.4f, 1.5f}));

    Primitives::Triangle triangle1 = Primitives::Triangle(
        Primitives::Point({2.5, 0.0, 0.0}),
        Primitives::Point({0.0, 0.0, 8.5}),
        Primitives::Point({0.0, 0.0, 0.0})
    );

    Primitives::Triangle triangle2 = Primitives::Triangle(
        Primitives::Point({2.5, 0.0, 0.0}),
        Primitives::Point({2.5, 0.0, 8.5}),
        Primitives::Point({0.0, 0.0, 8.5})
    );

    Primitives::Shape* shapes[] = { 
        &sphere1,
        &sphere2,
        &triangle1,
        &triangle2
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
            
            // Pixel size in world units
            float pixelWidth = filmplane.GetWidth() / width;
            float pixelHeight = filmplane.GetHeight() / height;

            // TODO: change this based on camera rotation
            dir = Primitives::Direction({
                // Pixel offset + offset to middle of pixel - offset to center film plane
                (pixelWidth * x) + (pixelWidth / 2) - filmplane.GetWidth() / 2,
                (pixelHeight * y) + (pixelHeight / 2) - filmplane.GetHeight() / 2,
                filmplane.GetDist(),
            }).normalize();

            Primitives::Ray ray = Primitives::Ray(
                Primitives::Point({0, 0, 0}),
                dir
            );

            for (Primitives::Shape* shape : shapes) {
                Primitives::IntersectionInfo intersection = shape->Intersect(ray);
                if (intersection.hit) {
                    color = intersection.color;
                }
            }

            // Set pixel color
            TGASetPixel(&image, x, y, color);
        }
    }

    TGADumpFile(&image, "out.tga");
}
