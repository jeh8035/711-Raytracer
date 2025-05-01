#ifndef WORLD_H
#define WORLD_H

#include "Libraries/matrix/matrix.h"

#include "Primitives/object.h"
#include "Primitives/camera.h"
#include "Primitives/light.h"
#include "Primitives/texture.h"

#include <memory>

class World {
    private:
        static Primitives::Camera camera;
        
        static std::vector<std::shared_ptr<Primitives::Object>> objects;
        static std::vector<Primitives::Light> lights;

        static constexpr uint32_t width = 1920;
        static constexpr uint32_t height = 1080;
        static constexpr float aspect_ratio = static_cast<float>(height)/static_cast<float>(width);
        static constexpr uint32_t supersample_amount = 5;

        static constexpr float epsilon = .0001f;

        static std::vector<std::vector<Primitives::Color>> irradiances;

        static std::vector<std::shared_ptr<Primitives::Texture>> textures;
        static std::vector<std::shared_ptr<Primitives::Material>> materials;

    public:
        static void RayTrace();
        
        static Primitives::IntersectionInfo CastRay(const Primitives::Ray& ray);

        static const std::vector<Primitives::Light>& GetLights() {return lights;}
        static const float& GetEpsilon() {return epsilon;}
        static const uint32_t& GetWidth() {return width;}
        static const uint32_t& GetHeight() {return height;}


        static const Primitives::Color GetBackgroundColor() { return Primitives::Color(0, 100, 100); }

    private:
        static void CreateObjects();
        static void TransformObjectsToCameraSpace();

        static void SingleTrace(u_int32_t y);

};

#endif