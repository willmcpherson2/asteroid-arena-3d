#ifndef OBJECT_H
#define OBJECT_H

#include "math.h"
#include <variant>
#include <vector>

struct Colour {
    void draw() const;

    double r { 0 };
    double g { 0 };
    double b { 0 };
};

struct Polygon {
    void draw() const;
    void add(Vec vertex, Vec normal);

    std::vector<Vec> vertices;
    std::vector<Vec> normals;
    Colour colour;
};

struct Line {
    void draw() const;
    void add(Vec vertex);

    std::vector<Vec> vertices;
    Colour colour;
};

using Shape = std::variant<Line, Polygon>;

struct Model {
    void draw() const;
    void add(Shape shape);
    void set_colour(Colour colour);

    std::vector<Shape> shapes;
};

struct Object {
    Object(Model model);
    void draw() const;
    void draw_camera(Vec focus) const;

    void look(double x_delta, double y_delta);
    void forward(double delta);

    Model model;
    Vec pos;
    Vec x { 1, 0, 0 };
    Vec y { 0, 1, 0 };
    Vec z { 0, 0, 1 };
};

#endif
