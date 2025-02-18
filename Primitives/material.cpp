
#include "material.h"
#include "intersection_info.h"
#include "../world.h"

namespace Primitives {
    Material::Material(Color _diffuse_color, Color _specular_color, float _phong_bg, float _phong_diffuse, float _phong_specular, float _phong_exponent) :
        diffuse_color(_diffuse_color),
        specular_color(_specular_color),
        phong_bg(_phong_bg),
        phong_diffuse(_phong_diffuse),
        phong_specular(_phong_specular),
        phong_exponent(_phong_exponent)
    {}

    Color Material::GetColor(Primitives::Ray ray, Primitives::IntersectionInfo intersection) const {
        Primitives::Point intersection_point = intersection.rayDist * ray.GetDirection();

        // Detect shadow
        Primitives::Direction dir_to_light = (World::GetLight().GetPosition() - intersection_point).normalize();

        Primitives::Ray ray_to_light = Primitives::Ray(
            intersection_point + (intersection.normal * World::GetEpsilon()),
            (World::GetLight().GetPosition() - intersection_point).normalize()
        );
        Primitives::IntersectionInfo light_intersection = World::CastRay(ray_to_light);

        if (!light_intersection.hit) {

            Primitives::Color diffuse = intersection.material.diffuse_color * intersection.material.phong_diffuse * World::GetLight().GetIntensity() * (dir_to_light * intersection.normal);
            Primitives::Color specular = intersection.material.specular_color * intersection.material.phong_specular * World::GetLight().GetIntensity() * pow( Primitives::ReflectRay(ray_to_light.GetDirection(), intersection.normal ) * -ray.GetDirection(), intersection.material.phong_exponent);
            return (diffuse + specular);
        }

        return Color(0.0f, 0.0f, 0.0f);
    }
}