#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "object.h"

namespace parameters {

namespace input {

    constexpr unsigned char forward = 'w';
    constexpr unsigned char back = 's';
    constexpr unsigned char left = 'a';
    constexpr unsigned char right = 'd';
    constexpr double mouse_sensitivity = 0.01;

}

namespace view {

    constexpr double fov = 90;
    constexpr double ratio = 1;
    constexpr double distance = 1000;
    constexpr double near_plane = 0.1;

}

namespace arena {

    constexpr int size = 240;
    constexpr int grid_divisions = 10;
    constexpr Colour colour { 0.5, 0.5, 0.5 };

}

namespace ship {

    constexpr double speed = 0.04;
    constexpr double camera_distance = 4;

}

}

#endif
