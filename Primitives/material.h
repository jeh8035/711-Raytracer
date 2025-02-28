#ifndef MATERIAL_H
#define MATERIAL_H

#include "primitives.h"
#include "texture.h"

namespace Primitives {

    struct IntersectionInfo;

    class Material {
        protected:
            std::shared_ptr<Texture> texture;

        public:
            Material(){};
            Material(std::shared_ptr<Texture> texture);

            virtual Color GetColor(Primitives::Ray ray, Primitives::IntersectionInfo intersection) const = 0;
    };

    class PhongMaterial : public Material {
        private:
            Color specular_color;
            float phong_bg;
            float phong_diffuse;
            float phong_specular;
            float phong_exponent;

        public:
            PhongMaterial(){};
            PhongMaterial(const std::shared_ptr<Texture> texture, const Color& _specular_color, const float& _phong_bg, const float& _phong_diffuse, const float& _phong_specular, const float& _phong_exponent);

            virtual Color GetColor(Primitives::Ray ray, Primitives::IntersectionInfo intersection) const;
    };
}

#endif