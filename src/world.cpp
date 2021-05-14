#include "world.h"
#include "gl-includes.h"
#include <algorithm>
#include <iostream>

// World

void World::simulate(int delta, Input& input)
{
    m_delta = delta;
    m_input = input;

    m_ship.simulate(delta, input);
}

void World::draw() const
{
    world_coordinates();
    m_ship.draw();
    draw_arena();
}

void World::draw_arena() const
{
    glColor3d(0.5, 0.5, 0.5);

    glPushMatrix();

    glScaled(arena_width, arena_height, arena_depth);

    Vec bot1 { -1, -1, -1 };
    Vec bot2 { -1, -1, 1 };
    Vec bot3 { 1, -1, 1 };
    Vec bot4 { 1, -1, -1 };
    Vec top1 { -1, 1, -1 };
    Vec top2 { -1, 1, 1 };
    Vec top3 { 1, 1, 1 };
    Vec top4 { 1, 1, -1 };

    glLineWidth(8);

    glBegin(GL_LINE_LOOP);
    bot1.draw();
    bot2.draw();
    bot3.draw();
    bot4.draw();
    glEnd();

    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    top4.draw();
    top3.draw();
    top2.draw();
    top1.draw();
    glEnd();

    glBegin(GL_LINES);
    top1.draw();
    bot1.draw();
    top2.draw();
    bot2.draw();
    top3.draw();
    bot3.draw();
    top4.draw();
    bot4.draw();
    glEnd();

    glPopMatrix();
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

// Ship

void Ship::simulate(int delta, Input& input)
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
