#include "Libraries/tinyga/tinyga.h"
#include "Libraries/matrix/matrix.h"

#include "Primitives/shape.h"
#include "Primitives/camera.h"

int main() {
    const unsigned int width = 1920;
    const unsigned int height = 1080;
    constexpr float aspect_ratio = static_cast<float>(height)/static_cast<float>(width);

    // Create image
    TGA image = TGANew(width, height, TGACOLOR(0,0,0));
    
    // Create camera
    Primitives::Camera camera = Primitives::Camera(
        Primitives::Point({1.1f, 1.9f, -5.0f}),
        Primitives::Direction({0.0f, 0.0f, 1.0f}),
        0.15f,
        0.15f * aspect_ratio,
        0.2f
    );

    // Define objects
    Primitives::Sphere sphere1 = Primitives::Sphere(
        TGACOLOR(0, 255, 0),
        0.5f,
        Primitives::Point({1.4f, 1.8f, 3.0f})
    );
    Primitives::Sphere sphere2 = Primitives::Sphere(
        TGACOLOR(255, 0, 0),
        0.5f,
        Primitives::Point({1.0f, 2.4f, 1.5f})
    );

    Primitives::Triangle triangle1 = Primitives::Triangle(
        TGACOLOR(0, 0, 255),
        Primitives::Point({2.5f, 0.0f, 0.0f}),
        Primitives::Point({0.0f, 0.0f, 8.5f}),
        Primitives::Point({0.0f, 0.0f, 0.0f})
    );

    Primitives::Triangle triangle2 = Primitives::Triangle(
        TGACOLOR(0, 0, 255),
        Primitives::Point({2.5, 0.0, 0.0}),
        Primitives::Point({2.5, 0.0, 8.5}),
        Primitives::Point({0.0, 0.0, 8.5})
    );

    Primitives::Cylinder cylinder = Primitives::Cylinder(
        TGACOLOR(255, 0, 0),
        Primitives::Point({1.5f, 2.4f, 1.5f}),
        Primitives::Point({1.0f, 2.4f, 2.5f}),
        0.3f
    );

    Primitives::Shape* shapes[] = { 
        // &sphere1,
        // &sphere2,
        &triangle1,
        &triangle2,
        &cylinder,
    };

    // Transform objects to camera space
    for (auto shape : shapes) {
        shape->Transform(camera.GetViewMatrix());
    }

    // Cast rays
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            TGAColor color = TGACOLOR(0, 0, 0);

            Primitives::Direction dir = Primitives::Direction();
            
            // Pixel size in world units
            float pixelWidth = camera.GetFilmplaneWidth() / width;
            float pixelHeight = camera.GetFilmplaneHeight() / height;

            // TODO: change this based on camera rotation
            dir = Primitives::Direction({
                // Pixel offset + offset to middle of pixel - offset to center film plane
                (pixelWidth * x) + (pixelWidth / 2) - camera.GetFilmplaneWidth() / 2,
                (pixelHeight * y) + (pixelHeight / 2) - camera.GetFilmplaneHeight() / 2,
                camera.GetFilmplaneDist(),
            }).normalize();

            Primitives::Ray ray = Primitives::Ray(
                Primitives::Point({0, 0, 0}),
                dir
            );

            // Intersect objects
            float currentMinDist = std::numeric_limits<float>().max();
            for (Primitives::Shape* shape : shapes) {
                Primitives::IntersectionInfo intersection = shape->Intersect(ray);
                if (intersection.hit && intersection.rayDist < currentMinDist) {
                    currentMinDist = intersection.rayDist;
                    color = intersection.color;
                }
            }

            // Set pixel color
            TGASetPixel(&image, x, y, color);
        }
    }

    TGADumpFile(&image, "out.tga");
}
