
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

    float PhongMaterial::GetTransparency() const {
        return transmission_constant;
    }

    Color PhongMaterial::GetColor(Primitives::Ray ray, Primitives::IntersectionInfo intersection, int depth) const {
        if (depth >= max_depth) {
            return World::GetBackgroundColor();
        }

        const Primitives::Point intersection_point = ray.GetPosition() + intersection.rayDist * ray.GetDirection();
        const Primitives::Point adjusted_intersection_point = intersection_point + (intersection.normal * World::GetEpsilon());
        
        // Color for this ray
        Primitives::Color color = Primitives::Color(0.0f, 0.0f, 0.0f);

        // Add contributions from lights
        for (Primitives::Light light : World::GetLights()) {
            const Primitives::Direction dir_to_light = (light.GetPosition() - intersection_point).normalize();

            // Shadow ray
            const Primitives::Ray ray_to_light = Primitives::Ray(
                adjusted_intersection_point,
                dir_to_light
            );
            const Primitives::IntersectionInfo light_intersection = World::CastRay(ray_to_light);

            // If shadow ray doesn't hit, calculate phong
            if (!light_intersection.hit || (light_intersection.hit && light_intersection.material->GetTransparency() > 0.0f)) {
                
                float light_normal_dot = dir_to_light * intersection.normal;
                
                if (light_normal_dot > 0.0f) {
                    float transparency = light_intersection.hit ? light_intersection.material->GetTransparency() : 1.0f;
                    Primitives::Color diffuse = texture->GetColor(intersection.u, intersection.v) * phong_diffuse * light.GetIntensity() * (dir_to_light * intersection.normal);
                    Primitives::Color specular = specular_color * phong_specular * light.GetIntensity() * pow( Primitives::ReflectRay(ray_to_light.GetDirection(), intersection.normal ) * -ray.GetDirection(), phong_exponent);
                    

                    color += (diffuse + specular) * transparency;
                }
            }

            // if (!light_intersection.hit) {
            //     Primitives::Color diffuse = texture->GetColor(intersection.u, intersection.v) * phong_diffuse * light.GetIntensity() * (dir_to_light * intersection.normal);
            //     Primitives::Color specular = specular_color * phong_specular * light.GetIntensity() * pow( Primitives::ReflectRay(ray_to_light.GetDirection(), intersection.normal ) * -ray.GetDirection(), phong_exponent);
            //     color += (diffuse + specular);
            // }
        }

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
            Primitives::Direction refract_ray;
            Primitives::Point inner_intersection_point = intersection_point;
            bool total_interal_reflection = false;
            if (intersection.is_inside) {
                refract_ray = Primitives::RefractRay(ray.GetDirection(), -intersection.normal, index_of_refraction, 1.0f, total_interal_reflection);
                inner_intersection_point += (intersection.normal * World::GetEpsilon()) * (total_interal_reflection ? -1.0 : 1.0);
            } else {
                refract_ray = Primitives::RefractRay(ray.GetDirection(), intersection.normal, 1.0f, index_of_refraction, total_interal_reflection);
                inner_intersection_point -= (intersection.normal * World::GetEpsilon()) * (total_interal_reflection ? -1.0 : 1.0);
            }

            Primitives::Ray trans_ray = Primitives::Ray(
                inner_intersection_point,
                refract_ray
            );
            Primitives::IntersectionInfo trans_result = World::CastRay(trans_ray);
            trans_result.is_inside = !intersection.is_inside;

            Primitives::Color transmissive_color;
            if (trans_result.hit) transmissive_color = trans_result.material->GetColor(trans_ray, trans_result, depth + 1);
            else transmissive_color = Color(0.0f, 0.0f, 0.0f);

            color += transmissive_color * transmission_constant;
        }

        return color;
    }
}