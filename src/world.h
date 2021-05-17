#ifndef WORLD_H
#define WORLD_H

#include "input.h"
#include "math.h"
#include "parameters.h"
#include <vector>

struct Colour {
    double r { 0 };
    double g { 0 };
    double b { 0 };
};

enum class DrawType {
    Triangles,
    Lines,
};

struct Object {
    Object(std::vector<Vec> model);
    void draw(DrawType draw_type, Colour colour) const;
    void draw_camera(Vec focus) const;

    void look(double x_delta, double y_delta);
    void forward(double delta);

    std::vector<Vec> model;
    Vec pos;
    Vec x { 1, 0, 0 };
    Vec y { 0, 1, 0 };
    Vec z { 0, 0, 1 };
};

class Arena {
public:
    Arena();
    void draw() const;
    void simulate(int delta, Input input);

private:
    static Object make_wall(double x_theta, double y_theta);

    Object m_top;
    Object m_bottom;
    Object m_left;
    Object m_right;
    Object m_front;
    Object m_back;
};

class Ship {
public:
    Ship();
    void draw() const;
    void simulate(int delta, Input input);

private:
    Object m_ship;
};

class World {
public:
    void draw() const;
    void simulate(int delta, Input input);

private:
    static void screen_coordinates();
    static void world_coordinates();

    int m_delta { 0 };
    Input m_input;

    Ship m_ship;
    Arena m_arena;
};

#endif
