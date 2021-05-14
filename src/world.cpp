#include "world.h"
#include "gl-includes.h"
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

std::vector<Vec> Arena::make_wall(double x_theta, double y_theta)
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

    return wall;
}

void Arena::draw() const
{
    glColor3d(0.5, 0.5, 0.5);

    glPushMatrix();

    glBegin(GL_LINES);
    for (Vec v : m_top) {
        v.draw();
    }
    for (Vec v : m_bottom) {
        v.draw();
    }
    for (Vec v : m_left) {
        v.draw();
    }
    for (Vec v : m_right) {
        v.draw();
    }
    for (Vec v : m_front) {
        v.draw();
    }
    for (Vec v : m_back) {
        v.draw();
    }
    glEnd();

    glPopMatrix();
}

// Ship

void Ship::simulate(int delta, Input input)
{
    double x_delta = delta * input_mouse_sensitivity * input.mouse_delta_x();
    double y_delta = delta * input_mouse_sensitivity * input.mouse_delta_y();

    if (input.forward()) {
        m_pos = m_pos + m_z * ship_speed * delta;
    }

    bool up_vector_top_hemisphere = m_y.theta({ 0, 1, 0 }) < 90;
    x_delta = up_vector_top_hemisphere ? x_delta : -x_delta;

    m_x = m_x.rotate_y(x_delta);
    m_y = m_y.rotate_y(x_delta);
    m_z = m_z.rotate_y(x_delta);
    m_z = m_z.rotate(m_x, y_delta);
    m_y = m_y.rotate(m_x, y_delta);

    assert(near(m_x.theta(m_y), 90));
    assert(near(m_x.theta(m_z), 90));
    assert(near(m_y.theta(m_z), 90));
}

void Ship::draw() const
{
    draw_camera();
    draw_ship();
}

void Ship::draw_camera() const
{
    Vec camera_pos = m_pos + m_z * -4;
    gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z, m_pos.x, m_pos.y, m_pos.z, m_y.x, m_y.y, m_y.z);

    glMatrixMode(GL_MODELVIEW);
}

void Ship::draw_ship() const
{
    glPushMatrix();

    glLineWidth(4);

    glBegin(GL_LINES);

    glColor3d(0, 0, 0.5);
    m_pos.draw();
    (m_pos + m_x).draw();

    glColor3d(0, 0.5, 0);
    m_pos.draw();
    (m_pos + m_y).draw();

    glColor3d(1, 0, 0);
    m_pos.draw();
    (m_pos + m_z).draw();

    glEnd();

    glPopMatrix();
}
