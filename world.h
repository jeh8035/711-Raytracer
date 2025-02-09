#include "Libraries/tinyga/tinyga.h"
#include "Libraries/matrix/matrix.h"

#include "Primitives/object.h"
#include "Primitives/camera.h"
#include "Primitives/light.h"

#include <memory>

class World {
    public:
        World(){};

        void RayTrace();
        Primitives::IntersectionInfo CastRay(const Primitives::Ray& ray);

    private:
        Primitives::Camera camera;
        
        std::vector<std::shared_ptr<Primitives::Object>> objects;
        Primitives::Light light;

        const uint32_t width = 1920;
        const uint32_t height = 1080;
        const float aspect_ratio = static_cast<float>(height)/static_cast<float>(width);

        void CreateObjects();
        void TransformObjectsToCameraSpace();
};