
#include "material.h"
#include "intersection_info.h"
#include "../world.h"

namespace Primitives {
    Material::Material(std::shared_ptr<Texture> _texture):
        texture(_texture)
    {}

    PhongMaterial::PhongMaterial(const std::shared_ptr<Texture> _texture, const Color& _specular_color, const float& _phong_diffuse, const float& _phong_specular, const float& _phong_exponent, const float& _reflection_constant, const float& _transmission_constant, const float& _index_of_refraction, const int& _max_depth) :
        Material(_texture),
        specular_color(_specular_color),
        phong_diffuse(_phong_diffuse),
        phong_specular(_phong_specular),
        phong_exponent(_phong_exponent),
        reflection_constant(_reflection_constant),
        transmission_constant(_transmission_constant),
        index_of_refraction(_index_of_refraction),
        max_depth(_max_depth)
    {}

    Color PhongMaterial::GetColor(Primitives::Ray ray, Primitives::IntersectionInfo intersection, int depth) const {
        if (depth >= max_depth) {
            return Color(0.0f, 0.0f, 0.0f);
        }

        const Primitives::Point intersection_point = ray.GetPosition() + intersection.rayDist * ray.GetDirection();
        const Primitives::Point adjusted_intersection_point = intersection_point + (intersection.normal * World::GetEpsilon());
        const Primitives::Direction dir_to_light = (World::GetLight().GetPosition() - intersection_point).normalize();

        // Color for this ray
        Primitives::Color color = Primitives::Color(0.0f, 0.0f, 0.0f);

        // Shadow ray
        const Primitives::Ray ray_to_light = Primitives::Ray(
            adjusted_intersection_point,
            dir_to_light
        );
        const Primitives::IntersectionInfo light_intersection = World::CastRay(ray_to_light);

        // If shadow ray doesn't hit, calculate phong
        if (!light_intersection.hit) {
            Primitives::Color diffuse = texture->GetColor(intersection.u, intersection.v) * phong_diffuse * World::GetLight().GetIntensity() * (dir_to_light * intersection.normal);
            Primitives::Color specular = specular_color * phong_specular * World::GetLight().GetIntensity() * pow( Primitives::ReflectRay(ray_to_light.GetDirection(), intersection.normal ) * -ray.GetDirection(), phong_exponent);
            color = diffuse + specular;
        }

        Primitives::Color reflectColor = Primitives::Color(0.0f, 0.0f, 0.0f);

        if (reflection_constant > 0.0f) {
            // Reflect ray
            Primitives::Ray reflected_ray = Primitives::Ray(
                adjusted_intersection_point,
                Primitives::ReflectRay(ray.GetDirection(), intersection.normal)
            );
            Primitives::IntersectionInfo reflection_result = World::CastRay(reflected_ray);
            
            // Result
            Primitives::Color reflection_color;
            if (reflection_result.hit) reflection_color = reflection_result.material->GetColor(reflected_ray, reflection_result, depth + 1);
            else reflection_color = Color(0.0f, 0.0f, 0.0f);

            color += reflection_color * reflection_constant;
        }

        if (transmission_constant > 0.0f) {
            // Transmission ray
            Primitives::Ray trans_ray = Primitives::Ray(
                adjusted_intersection_point,
                Primitives::RefractRay(ray.GetDirection(), intersection.normal, 1.0f, index_of_refraction)
            );
            Primitives::IntersectionInfo trans_result = World::CastRay(trans_ray);


            //color += ;
        }

        return color;
    }
}