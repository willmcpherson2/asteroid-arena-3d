#include "world.h"
#include "obj-loader.h"
#include <GLUT/glut.h>
#include <algorithm>
#include <iostream>

// World

void World::simulate(int delta, Input input)
{
    m_delta = delta;
    m_input = input;

    m_ship.simulate(delta, input);
}

void World::draw() const
{
    world_coordinates();
    m_ship.draw();
    m_arena.draw();
}

void World::screen_coordinates()
{
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glViewport(0, 0, width, height);
}

void World::world_coordinates()
{
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    double width_ratio = static_cast<double>(width) / static_cast<double>(height);
    double height_ratio = static_cast<double>(height) / static_cast<double>(width);
    double ratio = std::max(width_ratio, height_ratio);

    int view_width = static_cast<int>(width >= height ? width : width * ratio);
    int view_height = static_cast<int>(width >= height ? height * ratio : height);

    int view_x = 0;
    int view_y = 0;

    glViewport(view_x, view_y, view_width, view_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(view_fov, view_ratio, view_near_plane, view_distance);
}

// Arena

Arena::Arena()
    : m_top(make_wall(0, 270))
    , m_bottom(make_wall(0, 90))
    , m_left(make_wall(270, 0))
    , m_right(make_wall(90, 0))
    , m_front(make_wall(0, 180))
    , m_back(make_wall(0, 0))
{
}

Object Arena::make_wall(double x_theta, double y_theta)
{
    std::vector<Vec> wall;

    constexpr int grid_square_size = arena_size / arena_grid_divisions;

    for (int int_i = 0; int_i <= arena_size; int_i += grid_square_size) {
        double i = static_cast<double>(int_i);

        constexpr double offset = static_cast<double>(arena_size) * 0.5;

        Vec top = (Vec { i, 0, 0 } - offset).rotate_x(x_theta).rotate_y(y_theta);
        Vec bottom = (Vec { i, arena_size, 0 } - offset).rotate_x(x_theta).rotate_y(y_theta);
        Vec left = (Vec { 0, i, 0 } - offset).rotate_x(x_theta).rotate_y(y_theta);
        Vec right = (Vec { arena_size, i, 0 } - offset).rotate_x(x_theta).rotate_y(y_theta);

        wall.push_back(top);
        wall.push_back(bottom);
        wall.push_back(left);
        wall.push_back(right);
    }

    return Object(wall);
}

void Arena::draw() const
{
    DrawType draw_type = DrawType::Lines;
    Colour colour { 0.5, 0.5, 0.5 };

    m_top.draw(draw_type, colour);
    m_bottom.draw(draw_type, colour);
    m_left.draw(draw_type, colour);
    m_right.draw(draw_type, colour);
    m_front.draw(draw_type, colour);
    m_back.draw(draw_type, colour);
}

// Ship

Ship::Ship()
    : m_ship(load("models/point-cube.obj"))
{
}

void Ship::simulate(int delta, Input input)
{
    double x_delta = delta * input_mouse_sensitivity * input.mouse_delta_x();
    double y_delta = delta * input_mouse_sensitivity * input.mouse_delta_y();
    m_ship.look(x_delta, y_delta);

    if (input.forward()) {
        double forward_delta = ship_speed * delta;
        m_ship.forward(forward_delta);
    }
}

void Ship::draw() const
{
    Object camera = m_ship;
    camera.forward(-4);
    camera.draw_camera(m_ship.pos);

    m_ship.draw(DrawType::Triangles, { 1, 1, 1 });
}

// Object

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
