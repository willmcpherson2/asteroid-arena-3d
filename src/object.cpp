#include "object.h"
#include <GLUT/glut.h>

Object::Object(std::vector<Vec> model)
    : model(model)
{
}

void Object::draw_camera(Vec focus) const
{
    gluLookAt(pos.x, pos.y, pos.z, focus.x, focus.y, focus.z, y.x, y.y, y.z);

    glMatrixMode(GL_MODELVIEW);
}

void Object::draw(DrawType draw_type, Colour colour) const
{
    glColor3d(colour.r, colour.g, colour.b);

    glPushMatrix();

    Vec tilt_axis = z.flatten_y().cross(z);
    double tilt_rotation = z.flatten_y().theta(z);
    glRotated(tilt_rotation, tilt_axis.x, tilt_axis.y, tilt_axis.z);

    double spin_theta = z.flatten_y().theta({ 0, 0, 1 });
    double spin_rotation = z.x >= 0 ? spin_theta : -spin_theta;
    glRotated(spin_rotation, 0, 1, 0);

    double flip_rotation = y.y >= 0 ? 0 : 180;
    glRotated(flip_rotation, 1, 0, 0);
    glRotated(flip_rotation, 0, 1, 0);

    switch (draw_type) {
    case DrawType::Triangles:
        glBegin(GL_TRIANGLES);
        break;
    case DrawType::Lines:
        glBegin(GL_LINES);
        break;
    }

    for (Vec v : model) {
        v.draw();
    }

    glEnd();

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
