#ifndef FILMPLANE_H
#define FILMPLANE_H

#include "primitives.h"

namespace Primitives {

    class FilmPlane {
        private:
            Point position;
            float height;
            float width;

        public:
            FilmPlane(Point _position, float _height, float _width);

            Point GetPosition() {return position;}
            float GetWidth() {return width;}
            float GetHeight() {return height;}
    };

}

#endif