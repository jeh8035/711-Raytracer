
#include "texture.h"

namespace Primitives {
    PlainTexture::PlainTexture(const Color& _color) :
        color(_color)
    {}

    const Color PlainTexture::GetColor(const float& u, const float& v) const {
        return color;
    }

    TilingTexture::TilingTexture(const Color& _color1, const Color& _color2) :
        color1(_color1),
        color2(_color2)
    {}

    const Color TilingTexture::GetColor(const float& u, const float& v) const {
        const float mod_u = floor(u / 0.1f);
        const float mod_v = floor(v / 0.1f);
        const bool horizontal = static_cast<int>(mod_u) % 2 == 0;
        const bool vertical = static_cast<int>(mod_v) % 2 == 0;

        // Psuedo-random number from https://thebookofshaders.com/10/
        float rand = fmodf(sin(algebra::Vector2f({mod_u, mod_v}) * algebra::Vector2f({12.9898f,78.233f})) * 43758.5453123, 1.0f);
        rand = std::max(rand, 0.5f);

        return ((horizontal ^ vertical) ? color1 : color2) * rand;
    }

    ImageTexture::ImageTexture(const std::string& filename) {
        img.read(filename);
    }

    const Color ImageTexture::GetColor(const float& u, const float& v) const {
        return Color(
            img.red_at(static_cast<u_int32_t>(u * img.get_width()), static_cast<u_int32_t>(v * img.get_height())) / 255.0f,
            img.green_at(static_cast<u_int32_t>(u * img.get_width()), static_cast<u_int32_t>(v * img.get_height())) / 255.0f,
            img.blue_at(static_cast<u_int32_t>(u * img.get_width()), static_cast<u_int32_t>(v * img.get_height())) / 255.0f
        );
    }
}