#ifndef OBJECT_H
#define OBJECT_H

#include "math.h"
#include <vector>

struct Colour {
    double r { 0 };
    double g { 0 };
    double b { 0 };
    double a { 0 };
};

struct Pixel {
    unsigned char r { 0 };
    unsigned char g { 0 };
    unsigned char b { 0 };
    unsigned char a { 0 };
};

enum class Display {
    Outline,
    Solid,
    Texture,
};

struct Image {
    int width { 0 };
    int height { 0 };
    std::vector<Pixel> pixels;
};

struct Texture {
    unsigned id { 0 };
    Image image;
};

struct Material {
    void draw() const;
    void reset() const;

    Colour ambient;
    Colour diffuse;
    Colour specular;
    Colour emissive;
    Texture texture;
    Display display { Display::Solid };
};

struct Vertex {
    Vec vertex;
    Vec normal;
    Vec uv;

    void draw() const;
};

struct Polygon {
    void draw() const;
    void add(Vertex vertex);

    std::vector<Vertex> vertices;
    Material material;
    Display display { Display::Solid };
};

struct Model {
    void draw() const;
    void add(Polygon shape);

    std::vector<Polygon> polygons;
};

struct Object {
    Object() = default;
    Object(Model model);
    void draw() const;
    void draw_camera(Vec focus) const;

    void move(Vec pos);
    void scale(double size);
    void scale(Vec size);
    void rotate(Vec axis, double theta);
    void rotate_x(double theta);
    void rotate_y(double theta);
    void look(double x_delta, double y_delta);
    void set_colour(Colour colour);
    void set_display(Display display);

    Model model;
    Vec pos;
    Vec size { 1, 1, 1 };
    Vec x { 1, 0, 0 };
    Vec y { 0, 1, 0 };
    Vec z { 0, 0, 1 };
};

#endif
