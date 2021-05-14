#ifndef WORLD_H
#define WORLD_H

#include "input.h"
#include "math.h"
#include "parameters.h"
#include <vector>

class Arena {
public:
    Arena();
    void draw() const;
    void simulate(int delta, Input& input);

private:
    static std::vector<Vec> make_wall(double x_theta, double y_theta);

    std::vector<Vec> m_top;
    std::vector<Vec> m_bottom;
    std::vector<Vec> m_left;
    std::vector<Vec> m_right;
    std::vector<Vec> m_front;
    std::vector<Vec> m_back;
};

class Ship {
public:
    void draw() const;
    void draw_ship() const;
    void draw_camera() const;
    void simulate(int delta, Input& input);

private:
    Vec m_pos { 0, 0, 0 };
    Vec m_x { 1, 0, 0 };
    Vec m_y { 0, 1, 0 };
    Vec m_z { 0, 0, 1 };
};

class World {
public:
    void draw() const;
    void draw_arena() const;
    void simulate(int delta, Input& input);

private:
    static void screen_coordinates();
    static void world_coordinates();

    int m_delta { 0 };
    Input m_input;

    Ship m_ship;
    Arena m_arena;
};

#endif
