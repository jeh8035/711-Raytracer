#ifndef MATERIAL_H
#define MATERIAL_H

#include "primitives.h"

namespace Primitives {

    struct IntersectionInfo;

    class Material {

        public:
            Material(){};

            virtual Color GetColor(Primitives::Ray ray, Primitives::IntersectionInfo intersection) const = 0;
    };

    class PhongMaterial : public Material {
        private:
            Color diffuse_color;
            Color specular_color;
            float phong_bg;
            float phong_diffuse;
            float phong_specular;
            float phong_exponent;

        public:
            PhongMaterial(){};
            PhongMaterial(const Color& _diffuse_color, const Color& _specular_color, const float& _phong_bg, const float& _phong_diffuse, const float& _phong_specular, const float& _phong_exponent);

            virtual Color GetColor(Primitives::Ray ray, Primitives::IntersectionInfo intersection) const;
    };


    class TilingMaterial : public Material {

        public:
            virtual Color GetColor(Primitives::Ray ray, Primitives::IntersectionInfo intersection) const;
    };
}

#endif