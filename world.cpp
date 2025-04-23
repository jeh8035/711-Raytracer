#include "world.h"
#include "tonemapper.h"
#include "Primitives/material.h"
#include "Primitives/intersection_info.h"
#include "Libraries/libbmp/libbmp.h"

#include <thread>

Primitives::Camera World::camera;
std::vector<std::shared_ptr<Primitives::Object>> World::objects;
std::vector<Primitives::Light> World::lights;
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
            } else {
                irradiances[x][y] = World::GetBackgroundColor();
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
        //SingleTrace(y);
        threads.emplace_back(std::thread(SingleTrace, y));
    }

    for (std::thread& thread : threads) {
        thread.join();
    }

    {
        const float key_value = Tonemapper::CalculateLogAverage(irradiances);
        const ReinhardTonemapping tonemapper = ReinhardTonemapping(300.0f, key_value);
        tonemapper.Tonemap(irradiances);
    }

    // Create image
    BmpImg img(width, height);
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            img.set_pixel(
                x, height - 1 - y,
                static_cast<uint8_t>(std::min(255.0f, irradiances[x][y].red * 255.0f)),
                static_cast<uint8_t>(std::min(255.0f, irradiances[x][y].green * 255.0f)),
                static_cast<uint8_t>(std::min(255.0f, irradiances[x][y].blue * 255.0f))
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
    auto texture_red = textures.emplace_back(new Primitives::PlainTexture(
        Primitives::Color(1.0f, 0.0f, 0.0f)
    ));

    auto texture_tiled = textures.emplace_back(new Primitives::TilingTexture(
        Primitives::Color(0.0f, 1.0f, 1.0f),
        Primitives::Color(1.0f, 0.0f, 0.0f)
    ));

    auto texture_blue = textures.emplace_back(new Primitives::PlainTexture(
        Primitives::Color(0.0f, 0.0f, 1.0f)
    ));

    auto texture_img = textures.emplace_back(new Primitives::ImageTexture(
        "images/texture1.bmp"
    ));

    auto texture_mandlebrot = textures.emplace_back(new Primitives::MandelbrotTexture());

    // Materials
    auto sphere1_mat = materials.emplace_back(new Primitives::PhongMaterial(
        texture_mandlebrot,
        Primitives::Color(1.0, 1.0, 1.0),
        0.5f,
        0.5f,
        10.0f,
        1.0f,
        0.0f,
        1.0f,
        5
    ));

    auto sphere2_mat = materials.emplace_back(new Primitives::PhongMaterial(
        texture_img,
        Primitives::Color(1.0, 1.0, 1.0),
        0.5f,
        0.5f,
        10.0f,
        0.2f,
        0.8f,
        0.9f,
        10
    ));

    auto floor_mat = materials.emplace_back(new Primitives::PhongMaterial(
        texture_tiled,
        Primitives::Color(1.0, 1.0, 1.0),
        0.5f,
        0.5f,
        10.0f,
        0.0f,
        0.0f,
        2.0f,
        5
    ));

    auto cylinder_mat = materials.emplace_back(new Primitives::PhongMaterial(
        texture_blue,
        Primitives::Color(1.0, 1.0, 1.0),
        0.5f,
        0.5f,
        10.0f,
        0.7f,
        0.0f,
        2.0f,
        5
    ));

    // Sphere 1
    objects.emplace_back( new Primitives::Sphere(
        sphere1_mat,
        0.8f,
        Primitives::Point({1.4f, 1.8f, 3.0f})
    ));

    // Sphere 2
    objects.emplace_back( new Primitives::Sphere(
        sphere2_mat,
        0.5f,
        Primitives::Point({1.0f, 2.4f, 1.5f})
    ));

    // Floor
    objects.emplace_back( new Primitives::Triangle(
        floor_mat,
        Primitives::Point({0.0f, 0.0f, 0.0f}),
        Primitives::Point({4.5f, 0.0f, 0.0f}),
        Primitives::Point({0.0f, 0.0f, 21.5f})
    ));

    objects.emplace_back( new Primitives::Triangle(
        floor_mat,
        Primitives::Point({4.5, 0.0, 21.5}),
        Primitives::Point({0.0, 0.0, 21.5}),
        Primitives::Point({4.5, 0.0, 0.0})
    ));

    // Cylinder
    objects.emplace_back( new Primitives::Cylinder(
        cylinder_mat,
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
    lights = {
        Primitives::Light(
            {-1.0f, 6.0f, -3.0f},
            Primitives::Color(20.0f, 20.0f, 20.0f)
        ),
        Primitives::Light(
            {4.0f, 6.0f, -3.0f},
            Primitives::Color(1.0f, 1.0f, 1.0f)
        )
    };
}

void World::TransformObjectsToCameraSpace() {
    // Transform objects to camera space
    for (auto& object : objects) {
        object->Transform(camera.GetViewMatrix());
    }
    for (auto& light : lights) {
        light.Transform(camera.GetViewMatrix());
    }
}