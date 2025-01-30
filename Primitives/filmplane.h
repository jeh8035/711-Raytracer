#ifndef FILMPLANE_H
#define FILMPLANE_H

#include "primitives.h"

namespace Primitives {

    class FilmPlane {
        private:
            float dist;
            float height;
            float width;

        public:
            FilmPlane(float _dist, float _height, float _width);

            float GetDist() {return dist;}
            float GetWidth() {return width;}
            float GetHeight() {return height;}
    };

}

#endif