#include "world.h"
#include "Primitives/material.h"
#include "Primitives/intersection_info.h"
#include "Libraries/libbmp/libbmp.h"

#include <thread>

Primitives::Camera World::camera;
std::vector<std::shared_ptr<Primitives::Object>> World::objects;
Primitives::Light World::light;
constexpr uint32_t World::width;
constexpr uint32_t World::height;
constexpr float World::aspect_ratio;
constexpr uint32_t World::supersample_amount;
constexpr float World::epsilon;
std::vector<std::vector<Primitives::Color>> World::irradiances;
std::vector<std::shared_ptr<Primitives::Texture>> World::textures;
std::vector<std::shared_ptr<Primitives::Material>> World::materials;

void World::SingleTrace(u_int32_t y) {
    unsigned int rand_seed = y;

    for (uint32_t x = 0; x < width; x++) {
        for (uint32_t s = 0; s < supersample_amount; s++) {
            Primitives::Direction dir = Primitives::Direction();
            
            // Pixel size in world units
            float pixelWidth = camera.GetFilmplaneWidth() / width;
            float pixelHeight = camera.GetFilmplaneHeight() / height;
            
            // Random offsets within pixel for supersampling
            float s_offset_x = 0.0f;
            float s_offset_y = 0.0f;
            if (supersample_amount > 1) {
                s_offset_x = pixelWidth * (static_cast<float>(rand_r(&rand_seed)) / static_cast<float>(RAND_MAX)) - pixelWidth / 2.0f;
                s_offset_y = pixelHeight * (static_cast<float>(rand_r(&rand_seed)) / static_cast<float>(RAND_MAX)) - pixelHeight / 2.0f;
            }

            dir = Primitives::Direction({
                // Pixel offset + offset to middle of pixel - offset to center film plane
                (pixelWidth * x) + (pixelWidth / 2) + s_offset_x - camera.GetFilmplaneWidth() / 2,
                (pixelHeight * y) + (pixelHeight / 2) + s_offset_y - camera.GetFilmplaneHeight() / 2,
                camera.GetFilmplaneDist(),
            }).normalize();

            Primitives::Ray ray = Primitives::Ray(
                Primitives::Point({0, 0, 0}),
                dir
            );

            // Intersect objects
            Primitives::IntersectionInfo intersection = CastRay(ray);
            
            if (intersection.hit) {
                Primitives::Color color = intersection.material->GetColor(ray, intersection);
                irradiances[x][y] += color * (1.0f/supersample_amount);
            }
        }
    }
}

void World::RayTrace() {
    irradiances = std::vector<std::vector<Primitives::Color>>(width, std::vector<Primitives::Color>(height, Primitives::Color(0.0f, 0.0f, 0.0f)));
    materials = std::vector<std::shared_ptr<Primitives::Material>>();
    textures = std::vector<std::shared_ptr<Primitives::Texture>>();

    CreateObjects();
    TransformObjectsToCameraSpace();

    // Cast rays
    static std::vector<std::thread> threads;
    
    for (uint32_t y = 0; y < height; y++) {
        threads.emplace_back(std::thread(SingleTrace, y));
    }

    for (std::thread& thread : threads) {
        thread.join();
    }


    // Create image
    BmpImg img(width, height);

    // Tone mapping
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            img.set_pixel(
                x, height - y,
                static_cast<uint8_t>(std::min(irradiances[x][y].red * 255.0f, 255.0f)),
                static_cast<uint8_t>(std::min(irradiances[x][y].green * 255.0f, 255.0f)),
                static_cast<uint8_t>(std::min(irradiances[x][y].blue * 255.0f, 255.0f))
            );
        }
    }

    img.write("out.bmp");
}

Primitives::IntersectionInfo World::CastRay(const Primitives::Ray& ray) {
    float currentMinDist = std::numeric_limits<float>().max();
    Primitives::IntersectionInfo result;

    for (auto& object : objects) {
        Primitives::IntersectionInfo intersection = object->Intersect(ray);
        if (intersection.hit && intersection.rayDist < currentMinDist) {
            currentMinDist = intersection.rayDist;
            result = intersection;
            result.material = object->GetMaterial();
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

    //Textures
    textures.emplace_back(new Primitives::PlainTexture(
        Primitives::Color(1.0f, 0.0f, 0.0f)
    ));

    textures.emplace_back(new Primitives::TilingTexture(
        Primitives::Color(0.0f, 1.0f, 1.0f),
        Primitives::Color(1.0f, 0.0f, 0.0f)
    ));

    textures.emplace_back(new Primitives::PlainTexture(
        Primitives::Color(0.0f, 0.0f, 1.0f)
    ));

    textures.emplace_back(new Primitives::ImageTexture(
        "images/texture1.bmp"
    ));

    // Materials
    materials.emplace_back(new Primitives::PhongMaterial(
        textures[0],
        Primitives::Color(1.0f, 1.0f, 1.0f),
        0.0f,
        0.5f,
        0.5f,
        10.0f
    ));

    materials.emplace_back(new Primitives::PhongMaterial(
        textures[3],
        Primitives::Color(1.0f, 1.0f, 1.0f),
        0.0f,
        0.5f,
        0.5f,
        10.0f
    ));

    materials.emplace_back(new Primitives::PhongMaterial(
        textures[1],
        Primitives::Color(1.0f, 1.0f, 1.0f),
        0.0f,
        0.9f,
        0.1f,
        2.0f
    ));

    materials.emplace_back(new Primitives::PhongMaterial(
        textures[2],
        Primitives::Color(1.0f, 1.0f, 1.0f),
        0.0f,
        0.9f,
        0.1f,
        10.0f
    ));

    // Objects
    objects.emplace_back( new Primitives::Sphere(
        materials[0],
        0.5f,
        Primitives::Point({1.4f, 1.8f, 3.0f})
    ));

   objects.emplace_back( new Primitives::Sphere(
        materials[1],
        0.5f,
        Primitives::Point({1.0f, 2.4f, 1.5f})
    ));

    objects.emplace_back( new Primitives::Triangle(
        materials[2],
        Primitives::Point({0.0f, 0.0f, 0.0f}),
        Primitives::Point({4.5f, 0.0f, 0.0f}),
        Primitives::Point({0.0f, 0.0f, 11.5f})
    ));

    objects.emplace_back( new Primitives::Triangle(
        materials[2],
        Primitives::Point({4.5, 0.0, 11.5}),
        Primitives::Point({0.0, 0.0, 11.5}),
        Primitives::Point({4.5, 0.0, 0.0})
    ));

    objects.emplace_back( new Primitives::Cylinder(
        materials[3],
        Primitives::Point({0.0f, 2.4f, 2.5f}),
        Primitives::Point({0.2f, 0.4f, 3.6f}),
        0.3f
    ));

    // // Bunny model

    // // Load model
    // std::ifstream file;
    // file.open("data/bun000.ply");
    // // Skip header
    // std::string line;
    // u_int32_t vertex_count;
    // while (std::getline(file, line)) {
    //     if (line.substr(0, 14) == "element vertex") {
    //         vertex_count = std::stoi(line.substr(14));
    //     }

    //     if (line == "end_header") {
    //         break;
    //     }
    // }

    // u_int32_t coord_index = 0;
    // float coordinates[9];

    // for (u_int32_t i = 0; i < vertex_count; i++) {
    //     std::getline(file, line);

    //     std::stringstream stream = std::stringstream(line);
    //     std::string num;
    //     while (std::getline(stream, num, ' ')) {
    //         coordinates[coord_index] = std::stof(num);
    //         coord_index++;
    //     }
        
    //     if (coord_index % 9 == 0) {
    //         objects.emplace_back( new Primitives::Triangle(
    //             material1,
    //             Primitives::Point({coordinates[0], coordinates[1], coordinates[2]}),
    //             Primitives::Point({coordinates[3], coordinates[4], coordinates[5]}),
    //             Primitives::Point({coordinates[6], coordinates[7], coordinates[8]})
    //         ));
    //         coord_index = 0;
    //     }
    // }
    

    // Lights
    light = Primitives::Light(
        {-1.0f, 6.0f, -3.0f},
        Primitives::Color(1.0f, 1.0f, 1.0f)
    );
}

void World::TransformObjectsToCameraSpace() {
    // Transform objects to camera space
    for (auto& object : objects) {
        object->Transform(camera.GetViewMatrix());
    }
    light.Transform(camera.GetViewMatrix());
}