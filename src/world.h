#ifndef WORLD_H
#define WORLD_H

#include "input.h"
#include "math.h"
#include "parameters.h"

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
};

#endif
