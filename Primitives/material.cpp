
#include "material.h"
#include "intersection_info.h"
#include "../world.h"

namespace Primitives {
    PhongMaterial::PhongMaterial(const Color& _diffuse_color, const Color& _specular_color, const float& _phong_bg, const float& _phong_diffuse, const float& _phong_specular, const float& _phong_exponent) :
        diffuse_color(_diffuse_color),
        specular_color(_specular_color),
        phong_bg(_phong_bg),
        phong_diffuse(_phong_diffuse),
        phong_specular(_phong_specular),
        phong_exponent(_phong_exponent)
    {}

    Color PhongMaterial::GetColor(Primitives::Ray ray, Primitives::IntersectionInfo intersection) const {
        Primitives::Point intersection_point = intersection.rayDist * ray.GetDirection();

        // Detect shadow
        Primitives::Direction dir_to_light = (World::GetLight().GetPosition() - intersection_point).normalize();

        Primitives::Ray ray_to_light = Primitives::Ray(
            intersection_point + (intersection.normal * World::GetEpsilon()),
            (World::GetLight().GetPosition() - intersection_point).normalize()
        );
        Primitives::IntersectionInfo light_intersection = World::CastRay(ray_to_light);

        if (!light_intersection.hit) {

            Primitives::Color diffuse = diffuse_color * phong_diffuse * World::GetLight().GetIntensity() * (dir_to_light * intersection.normal);
            Primitives::Color specular = specular_color * phong_specular * World::GetLight().GetIntensity() * pow( Primitives::ReflectRay(ray_to_light.GetDirection(), intersection.normal ) * -ray.GetDirection(), phong_exponent);
            return (diffuse + specular);
        }

        return Color(0.0f, 0.0f, 0.0f);
    }


    Color TilingMaterial::GetColor(Primitives::Ray ray, Primitives::IntersectionInfo intersection) const {
        return Color(1.0f, 0.0f, 0.0f);
    }
}