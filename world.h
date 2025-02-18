#include "Libraries/tinyga/tinyga.h"
#include "Libraries/matrix/matrix.h"

#include "Primitives/object.h"
#include "Primitives/camera.h"
#include "Primitives/light.h"

#include <memory>

class World {
    private:
        static Primitives::Camera camera;
        
        static std::vector<std::shared_ptr<Primitives::Object>> objects;
        static Primitives::Light light;

        static constexpr uint32_t width = 1280;
        static constexpr uint32_t height = 720;
        static constexpr float aspect_ratio = static_cast<float>(height)/static_cast<float>(width);
        static constexpr uint32_t supersample_amount = 4;

        static constexpr float epsilon = .0001f;

        static std::vector<std::vector<Primitives::Color>> irradiances;

    public:
        static void RayTrace();
        
        static Primitives::IntersectionInfo CastRay(const Primitives::Ray& ray);

        static const Primitives::Light& GetLight() {return light;}
        static const float GetEpsilon() {return epsilon;}

    private:
        static void CreateObjects();
        static void TransformObjectsToCameraSpace();
};