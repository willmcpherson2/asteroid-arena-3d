#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "object.h"

// Input

constexpr unsigned char input_forward = 'w';
constexpr unsigned char input_back = 's';
constexpr unsigned char input_left = 'a';
constexpr unsigned char input_right = 'd';
constexpr double input_mouse_sensitivity = 0.01;

// View

constexpr double view_fov = 90;
constexpr double view_ratio = 1;
constexpr double view_distance = 1000;
constexpr double view_near_plane = 0.1;

// Arena

constexpr int arena_size = 240;
constexpr int arena_grid_divisions = 10;
constexpr Colour arena_colour { 0.5, 0.5, 0.5 };

// Ship

constexpr double ship_speed = 0.04;

#endif
