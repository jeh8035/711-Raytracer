#ifndef TEXTURE_H
#define TEXTURE_H

#include "../Libraries/libbmp/libbmp.h"

#include "primitives.h"

namespace Primitives {

    class Texture {

        public:
            Texture(){};

            virtual const Color GetColor(const float& u, const float& v) const = 0;
    };

    class PlainTexture : public Texture {
        private:
            Color color;

        public:
            PlainTexture(){};
            PlainTexture(const Color& _color);

            virtual const Color GetColor(const float& u, const float& v) const;
    };

    class TilingTexture : public Texture {
        private:
            Color color1;
            Color color2;

        public:
            TilingTexture(){};
            TilingTexture(const Color& _color1, const Color& _color2);

            virtual const Color GetColor(const float& u, const float& v) const;
    };

    class ImageTexture : public Texture {
        private:
            BmpImg img;

        public:
            ImageTexture(){};
            ImageTexture(const std::string& filename);

            virtual const Color GetColor(const float& u, const float& v) const;
    };

    class MandelbrotTexture : public Texture {
        public:
            MandelbrotTexture(){};
            virtual const Color GetColor(const float& u, const float& v) const;
    };
}

#endif