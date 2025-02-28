
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
        bool horizontal = static_cast<int>(floor(u / 0.1f)) % 2 == 0;
        bool vertical = static_cast<int>(floor(v / 0.1f)) % 2 == 0;

        return (horizontal ^ vertical) ? color1 : color2;
    }

    ImageTexture::ImageTexture(const std::string& _filename) :
        filename(_filename)
    {}

    const Color ImageTexture::GetColor(const float& u, const float& v) const {
        return Color(fmod(u * v, 1.0f), 0.0f, 0.0f);
    }
}