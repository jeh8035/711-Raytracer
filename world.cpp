#include "world.h"

#include <iostream>

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
            
            if (intersection.hit) {
                Primitives::Point intersection_point = intersection.rayDist * ray.GetDirection();

                // Detect shadow
                Primitives::Direction dir_to_light = (light.GetPosition() - intersection_point).normalize();

                Primitives::Ray ray_to_light = Primitives::Ray(
                    intersection_point + (intersection.normal * .00001f),
                    (light.GetPosition() - intersection_point).normalize()
                );
                Primitives::IntersectionInfo light_intersection = CastRay(ray_to_light);

                if (!light_intersection.hit) {

                    Primitives::Color diffuse = intersection.object->GetMaterial().diffuse_color * intersection.object->GetMaterial().phong_diffuse * 1.0f * (dir_to_light * intersection.normal);
                    Primitives::Color specular = intersection.object->GetMaterial().specular_color * intersection.object->GetMaterial().phong_specular * 1.0f * pow( Primitives::ReflectRay(ray_to_light.GetDirection(), intersection.normal ) * -ray.GetDirection(), intersection.object->GetMaterial().phong_exponent);
                    Primitives::Color intensity = diffuse + specular;

                    color = TGACOLOR(
                        static_cast<uint8_t>(intensity.red * 255),
                        static_cast<uint8_t>(intensity.green * 255),
                        static_cast<uint8_t>(intensity.blue * 255)
                    );

                    // Normal colors
                    // color = TGACOLOR(
                    //     static_cast<uint8_t>((intersection.normal.x() + 1) * 128),
                    //     static_cast<uint8_t>((intersection.normal.y() + 1) * 128),
                    //     static_cast<uint8_t>((intersection.normal.z() + 1) * 128)
                    // );
                }
            }

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
            result.object = object;
        }
    }

    return result;
}

void World::CreateObjects() {
    // Camera
    camera = Primitives::Camera(
        Primitives::Point({1.1f, 1.9f, -5.0f}),
        Primitives::Direction({0.0f, 0.0f, 1.0f}),
        0.15f,
        0.15f * aspect_ratio,
        0.2f
    );

    // Materials
    Primitives::Material material1 = Primitives::Material(
        Primitives::Color(1.0f, 0.0f, 0.0f),
        Primitives::Color(1.0f, 1.0f, 1.0f),
        0.0f,
        0.5f,
        0.5f,
        10.0f
    );

    Primitives::Material material2 = Primitives::Material(
        Primitives::Color(1.0f, 0.0f, 0.0f),
        Primitives::Color(1.0f, 1.0f, 1.0f),
        0.0f,
        0.5f,
        0.5f,
        10.0f
    );

    Primitives::Material material3 = Primitives::Material(
        Primitives::Color(0.0f, 1.0f, 0.0f),
        Primitives::Color(1.0f, 1.0f, 1.0f),
        0.0f,
        0.5f,
        0.5f,
        10.0f
    );

    Primitives::Material material4 = Primitives::Material(
        Primitives::Color(0.0f, 0.0f, 1.0f),
        Primitives::Color(1.0f, 1.0f, 1.0f),
        0.0f,
        0.5f,
        0.5f,
        10.0f
    );

    // Objects
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

    // Lights
    light = Primitives::Light({-1.0f, 6.0f, -3.0f});
}

void World::TransformObjectsToCameraSpace() {
    // Transform objects to camera space
    for (auto& object : objects) {
        object->Transform(camera.GetViewMatrix());
    }
    light.Transform(camera.GetViewMatrix());
}