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
        static Primitives::Light light;

        static constexpr uint32_t width = 1280;
        static constexpr uint32_t height = 720;
        static constexpr float aspect_ratio = static_cast<float>(height)/static_cast<float>(width);
        static constexpr uint32_t supersample_amount = 5;

        static constexpr float epsilon = .0001f;

        static std::vector<std::vector<Primitives::Color>> irradiances;

        static std::vector<std::shared_ptr<Primitives::Texture>> textures;
        static std::vector<std::shared_ptr<Primitives::Material>> materials;

    public:
        static void RayTrace();
        
        static Primitives::IntersectionInfo CastRay(const Primitives::Ray& ray);

        static const Primitives::Light& GetLight() {return light;}
        static const float GetEpsilon() {return epsilon;}

    private:
        static void CreateObjects();
        static void TransformObjectsToCameraSpace();

        static void SingleTrace(u_int32_t y);
};