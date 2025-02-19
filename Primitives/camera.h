#ifndef CAMERA_H
#define CAMERA_H

#include "primitives.h"

namespace Primitives {

    class Camera {
        private:
            Point position;
            Direction lookat;

            algebra::Matrix4f transform_matrix;

            float filmplane_width;
            float filmplane_height;
            float filmplane_dist;

        public:
            Camera(Point _position, Point _lookat, float _filmplane_width, float _filmplane_height, float _filmplane_dist);
            Camera() {};

            float GetFilmplaneWidth() const {return filmplane_width;}
            float GetFilmplaneHeight() const {return filmplane_height;}
            float GetFilmplaneDist() const {return filmplane_dist;}

            const algebra::Matrix4f& GetViewMatrix() const {return transform_matrix;}
    };

}

#endif