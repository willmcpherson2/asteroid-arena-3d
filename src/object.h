#ifndef OBJECT_H
#define OBJECT_H

#include "math.h"
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

#endif
