#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "object.h"

namespace parameters {

namespace input {

    constexpr unsigned char forward = 'w';
    constexpr unsigned char quit1 = 'q';
    constexpr unsigned char quit2 = 27; // Esc
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
    constexpr Colour warning_colour { 1, 0, 0.2 };
    constexpr double warning_distance = 100;

}

namespace ship {

    constexpr double speed = 0.04;
    constexpr double camera_distance = 4;

}

}

#endif
