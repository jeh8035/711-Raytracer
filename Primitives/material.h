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

            virtual Color GetColor(Primitives::Ray ray, Primitives::IntersectionInfo intersection, int depth = 0) const = 0;
    };

    class PhongMaterial : public Material {
        private:
            Color specular_color;
            float phong_diffuse;
            float phong_specular;
            float phong_exponent;

            float reflection_constant;
            float transmission_constant;
            int max_depth;

        public:
            PhongMaterial(){};
            PhongMaterial(const std::shared_ptr<Texture> _texture, const Color& _specular_color, const float& _phong_diffuse, const float& _phong_specular, const float& _phong_exponent, const float& _reflection_constant, const float& _transmission_constant, const int& _max_depth);

            virtual Color GetColor(Primitives::Ray ray, Primitives::IntersectionInfo intersection, int depth = 0) const;
    };
}

#endif