#include "object.h"
#include <GLUT/glut.h>

Object::Object(Model model)
    : model(model)
{
}

void Object::draw_camera(Vec focus) const
{
    glMatrixMode(GL_PROJECTION);
    gluLookAt(pos.x, pos.y, pos.z, focus.x, focus.y, focus.z, y.x, y.y, y.z);
    glMatrixMode(GL_MODELVIEW);
}

void Object::draw_light() const
{
    float ambient[] = { 0.5, 0.5, 0.5, 1 };
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);

    float diffuse[] = { 0.5, 0.5, 0.5, 1 };
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);

    float specular[] = { 0.5, 0.5, 0.5, 1 };
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);

    float position[] = { static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(pos.z), 1 };
    glLightfv(GL_LIGHT1, GL_POSITION, position);

    float spot_direction[] = { static_cast<float>(z.x), static_cast<float>(z.y), static_cast<float>(z.z) };
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);

    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45);
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

    glScaled(size.x, size.y, size.z);

    model.draw();

    glPopMatrix();
}

void Object::move(Vec pos)
{
    this->pos = this->pos + x * pos.x;
    this->pos = this->pos + y * pos.y;
    this->pos = this->pos + z * pos.z;
}

void Object::scale(double size)
{
    this->size = this->size * Vec { size, size, size };
}

void Object::scale(Vec size)
{
    this->size = this->size * size;
}

void Object::rotate(Vec axis, double theta)
{
    x = x.rotate(axis, theta);
    y = y.rotate(axis, theta);
    z = z.rotate(axis, theta);
}

void Object::rotate_x(double theta)
{
    x = x.rotate_x(theta);
    y = y.rotate_x(theta);
    z = z.rotate_x(theta);
}

void Object::rotate_y(double theta)
{
    x = x.rotate_y(theta);
    y = y.rotate_y(theta);
    z = z.rotate_y(theta);
}

void Object::look(double x_delta, double y_delta)
{
    bool up_vector_top_hemisphere = y.theta({ 0, 1, 0 }) < 90;
    x_delta = up_vector_top_hemisphere ? x_delta : -x_delta;

    rotate_y(x_delta);
    rotate(x, y_delta);

    assert(near(x.theta(y), 90));
    assert(near(x.theta(z), 90));
    assert(near(y.theta(z), 90));
}

void Object::set_colour(Colour colour)
{
    for (auto& polygon : model.polygons) {
        polygon.colour = colour;
    }
}

void Object::set_display(Display display)
{
    for (auto& polygon : model.polygons) {
        polygon.display = display;
    }
}

void Model::draw() const
{
    for (auto polygon : polygons) {
        polygon.draw();
    }
}

void Model::add(Polygon polygon)
{
    polygons.push_back(polygon);
}

void Polygon::add(Vertex vertex)
{
    vertices.push_back(vertex);
}

void Polygon::draw() const
{
    switch (display) {
    case Display::Solid:
        glBegin(GL_POLYGON);
        break;
    case Display::Outline:
        glBegin(GL_LINE_LOOP);
        break;
    }

    for (auto vertex : vertices) {
        vertex.draw();
    }

    glEnd();
}

void Vertex::draw() const
{
    normal.draw_normal();
    vertex.draw();
}

void Colour::draw() const
{
    glColor3d(r, g, b);
}
