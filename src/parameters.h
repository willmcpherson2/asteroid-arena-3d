#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "object.h"

namespace parameters {

namespace input {
    constexpr unsigned char fire = ' ';
    constexpr unsigned char forward = 'w';
    constexpr unsigned char left = 'a';
    constexpr unsigned char right = 'd';
    constexpr unsigned char back = 's';
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
    constexpr int size = 400;
    constexpr double warning_size = 200;
    constexpr double skybox_size = 600;
}

namespace ship {
    constexpr double speed = 0.06;
    constexpr double size = 2;
    constexpr int fire_interval = 500;
    constexpr Vec forward_camera_pos { 0, 0.5, -2 };
    constexpr Vec left_camera_pos { -2, 0.5, 0 };
    constexpr Vec right_camera_pos { 2, 0.5, 0 };
    constexpr Vec back_camera_pos { 0, 0.5, 2 };
}

namespace bullet {
    constexpr double speed = 0.2;
    constexpr double size = 0.5;
    constexpr double damage = 4;
}

namespace particle {
    constexpr int amount = 3;
    constexpr double speed = 0.005;
    constexpr double size = 4;
    constexpr double duration = 5000;
}

namespace asteroid {
    constexpr double min_size = 4;
    constexpr double max_size = 24;
    constexpr double min_speed = 0.04;
    constexpr double max_speed = 0.08;
    constexpr double rotation_speed = 0.5;
    constexpr int wave_interval = 10000;
    constexpr double perturbation = 0.04;
}

}

#endif
