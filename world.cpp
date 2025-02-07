#include "world.h"

void World::RayTrace() {

    CreateObjects();
    TransformObjectsToCameraSpace();

    // Create image
    TGA image = TGANew(width, height, TGACOLOR(0,0,0));

    // Cast rays
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            TGAColor color = TGACOLOR(0, 0, 0);

            Primitives::Direction dir = Primitives::Direction();
            
            // Pixel size in world units
            float pixelWidth = camera.GetFilmplaneWidth() / width;
            float pixelHeight = camera.GetFilmplaneHeight() / height;

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
            Primitives::IntersectionInfo intersection = CastRay(ray);
            
            // color = TGACOLOR(
            //     static_cast<uint8_t>((std::max(0.0f, intersection.normal.x())) * 255),
            //     static_cast<uint8_t>((std::max(0.0f, intersection.normal.y())) * 255),
            //     static_cast<uint8_t>((std::max(0.0f, intersection.normal.z())) * 255)
            // );

            // color = TGACOLOR(
            //     static_cast<uint8_t>((intersection.normal.x() + 1) * 128),
            //     static_cast<uint8_t>((intersection.normal.y() + 1) * 128),
            //     static_cast<uint8_t>((intersection.normal.z() + 1) * 128)
            // );

            color = TGACOLOR(
                static_cast<uint8_t>(intersection.irradiance.red * 255),
                static_cast<uint8_t>(intersection.irradiance.green * 255),
                static_cast<uint8_t>(intersection.irradiance.blue * 255)
            );

            // Set pixel color
            TGASetPixel(&image, x, y, color);
        }
    }

    TGADumpFile(&image, "out.tga");
}

Primitives::IntersectionInfo World::CastRay(const Primitives::Ray& ray) {
    float currentMinDist = std::numeric_limits<float>().max();
    Primitives::IntersectionInfo result;

    for (auto& object : objects) {
        Primitives::IntersectionInfo intersection = object->Intersect(ray);
        if (intersection.hit && intersection.rayDist < currentMinDist) {
            currentMinDist = intersection.rayDist;
            result = intersection;
        }
    }

    return result;
}

void World::CreateObjects() {
    // Create camera
    camera = Primitives::Camera(
        Primitives::Point({1.1f, 1.9f, -5.0f}),
        Primitives::Direction({0.0f, 0.0f, 1.0f}),
        0.15f,
        0.15f * aspect_ratio,
        0.2f
    );

    Primitives::Material material1 = Primitives::Material(
        Primitives::Color(1.0f, 0.0f, 0.0f),
        Primitives::Color(1.0f, 1.0f, 1.0f),
        0.0f,
        0.5f,
        0.5f,
        2.0f
    );

    Primitives::Material material2 = Primitives::Material(
        Primitives::Color(1.0f, 0.0f, 0.0f),
        Primitives::Color(1.0f, 1.0f, 1.0f),
        0.0f,
        0.5f,
        0.5f,
        2.0f
    );

    Primitives::Material material3 = Primitives::Material(
        Primitives::Color(0.0f, 1.0f, 0.0f),
        Primitives::Color(1.0f, 1.0f, 1.0f),
        0.0f,
        0.5f,
        0.5f,
        2.0f
    );

    Primitives::Material material4 = Primitives::Material(
        Primitives::Color(0.0f, 0.0f, 1.0f),
        Primitives::Color(1.0f, 1.0f, 1.0f),
        0.0f,
        0.5f,
        0.5f,
        2.0f
    );

    // Define objects
    objects.emplace_back( new Primitives::Sphere(
        material1,
        0.5f,
        Primitives::Point({1.4f, 1.8f, 3.0f})
    ));

   objects.emplace_back( new Primitives::Sphere(
        material2,
        0.5f,
        Primitives::Point({1.0f, 2.4f, 1.5f})
    ));

    objects.emplace_back( new Primitives::Triangle(
        material3,
        Primitives::Point({4.5f, 0.0f, 0.0f}),
        Primitives::Point({0.0f, 0.0f, 11.5f}),
        Primitives::Point({0.0f, 0.0f, 0.0f})
    ));

    objects.emplace_back( new Primitives::Triangle(
        material3,
        Primitives::Point({4.5, 0.0, 0.0}),
        Primitives::Point({4.5, 0.0, 11.5}),
        Primitives::Point({0.0, 0.0, 11.5})
    ));

    objects.emplace_back( new Primitives::Cylinder(
        material4,
        Primitives::Point({0.0f, 2.4f, 2.5f}),
        Primitives::Point({0.2f, 0.4f, 3.6f}),
        0.3f
    ));
}

void World::TransformObjectsToCameraSpace() {
    // Transform objects to camera space
    for (auto& object : objects) {
        object->Transform(camera.GetViewMatrix());
    }
}