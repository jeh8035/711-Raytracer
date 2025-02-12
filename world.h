#include "Libraries/tinyga/tinyga.h"
#include "Libraries/matrix/matrix.h"

#include "Primitives/object.h"
#include "Primitives/camera.h"
#include "Primitives/light.h"

#include <memory>

class World {
    private:
        Primitives::Camera camera;
        
        std::vector<std::shared_ptr<Primitives::Object>> objects;
        Primitives::Light light;

        const uint32_t width = 1280;
        const uint32_t height = 720;
        const float aspect_ratio = static_cast<float>(height)/static_cast<float>(width);
        const uint32_t supersample_amount = 4;

        std::vector<std::vector<Primitives::Color>> irradiances;

    public:
        World() :
            irradiances(std::vector<std::vector<Primitives::Color>>(width, std::vector<Primitives::Color>(height, Primitives::Color(0.0f, 0.0f, 0.0f))))
        {}

        void RayTrace();
        
        Primitives::IntersectionInfo CastRay(const Primitives::Ray& ray);

    private:
        void CreateObjects();
        void TransformObjectsToCameraSpace();
};