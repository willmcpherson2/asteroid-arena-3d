#include "object.h"
#include <GLUT/glut.h>

Object::Object(Model model)
    : model(model)
{
}

void Object::draw_camera(Vec focus) const
{
    gluLookAt(pos.x, pos.y, pos.z, focus.x, focus.y, focus.z, y.x, y.y, y.z);
    glMatrixMode(GL_MODELVIEW);
}

void Object::draw() const
{
    glPushMatrix();

    glTranslated(pos.x, pos.y, pos.z);

    Vec tilt_axis = z.flatten_y().cross(z);
    double tilt_rotation = z.flatten_y().theta(z);
    glRotated(tilt_rotation, tilt_axis.x, tilt_axis.y, tilt_axis.z);

    double spin_theta = z.flatten_y().theta({ 0, 0, 1 });
    double spin_rotation = z.x >= 0 ? spin_theta : -spin_theta;
    glRotated(spin_rotation, 0, 1, 0);

    double flip_rotation = y.y >= 0 ? 0 : 180;
    glRotated(flip_rotation, 1, 0, 0);
    glRotated(flip_rotation, 0, 1, 0);

    model.draw();

    glPopMatrix();
}

void Object::look(double x_delta, double y_delta)
{
    bool up_vector_top_hemisphere = y.theta({ 0, 1, 0 }) < 90;
    x_delta = up_vector_top_hemisphere ? x_delta : -x_delta;

    x = x.rotate_y(x_delta);
    y = y.rotate_y(x_delta);
    z = z.rotate_y(x_delta);
    z = z.rotate(x, y_delta);
    y = y.rotate(x, y_delta);

    assert(near(x.theta(y), 90));
    assert(near(x.theta(z), 90));
    assert(near(y.theta(z), 90));
}

void Object::forward(double delta)
{
    pos = pos + z * delta;
}

void Model::draw() const
{
    for (auto shape : shapes) {
        std::visit([](const auto& shape) { shape.draw(); }, shape);
    }
}

void Model::add(Shape shape)
{
    shapes.push_back(shape);
}

void Model::set_colour(Colour colour)
{
    for (auto& shape : shapes) {
        std::visit([colour](auto& shape) { shape.colour = colour; }, shape);
    }
}

void Line::draw() const
{
    colour.draw();

    glBegin(GL_LINE_LOOP);
    for (auto v : vertices) {
        v.draw();
    }
    glEnd();
}

void Line::add(Vec vertex)
{
    vertices.push_back(vertex);
}

void Polygon::add(Vec vertex, Vec normal)
{
    vertices.push_back(vertex);
    normals.push_back(normal);
}

void Polygon::draw() const
{
    assert(vertices.size() == normals.size());

    colour.draw();

    glBegin(GL_POLYGON);
    for (size_t i = 0; i < vertices.size(); ++i) {
        Vec normal = normals[i];
        Vec vertex = vertices[i];
        normal.draw_normal();
        vertex.draw();
    }
    glEnd();
}

void Colour::draw() const
{
    glColor3d(r, g, b);
}
