#include "world.h"
#include "gl-includes.h"
#include <algorithm>
#include <iostream>

void World::simulate(int delta, Input& input)
{
    m_delta = delta;
    m_input = input;

    m_camera.simulate(delta, input);
}

void Camera::simulate(int delta, Input& input)
{
    if (input.forward()) {
        m_pos = m_pos + m_z * ship_speed;
    }

    double x_delta = delta * input_mouse_sensitivity * input.mouse_delta_x();
    double y_delta = delta * input_mouse_sensitivity * input.mouse_delta_y();

    m_z = m_z.rotate(m_y, x_delta);
    m_x = m_x.rotate(m_y, x_delta);
    m_z = m_z.rotate(m_x, y_delta);
    m_y = m_y.rotate(m_x, y_delta);

    assert(near(m_z.length(), 1.0));
    assert(near(m_x.length(), 1.0));
    assert(near(m_y.length(), 1.0));
    assert(near(m_z.dot(m_x), 0.0));
    assert(near(m_z.dot(m_y), 0.0));
    assert(near(m_x.dot(m_y), 0.0));
}

void World::draw() const
{
    world_coordinates();
    m_camera.draw();
    draw_cube();
}

void Camera::draw() const
{
    Vec focus = m_pos + m_z;

    gluLookAt(m_pos.x, m_pos.y, m_pos.z, focus.x, focus.y, focus.z, m_y.x, m_y.y, m_y.z);

    glMatrixMode(GL_MODELVIEW);
}

void World::draw_cube() const
{
    auto cube = [](double size) {
        glPushMatrix();

        glScaled(size, size, size);

        Vec bot1 { -1.0, -1.0, -1.0 };
        Vec bot2 { -1.0, -1.0, 1.0 };
        Vec bot3 { 1.0, -1.0, 1.0 };
        Vec bot4 { 1.0, -1.0, -1.0 };
        Vec top1 { -1.0, 1.0, -1.0 };
        Vec top2 { -1.0, 1.0, 1.0 };
        Vec top3 { 1.0, 1.0, 1.0 };
        Vec top4 { 1.0, 1.0, -1.0 };

        glBegin(GL_LINE_LOOP);
        bot1.draw();
        bot2.draw();
        bot3.draw();
        bot4.draw();
        glEnd();

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
    };

    glLineWidth(2.0);

    glColor3d(1.0, 0.5, 0.5);
    cube(10.0);

    glColor3d(0.5, 0.5, 1.0);
    cube(100.0);
}

void World::screen_coordinates()
{
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    glLoadIdentity();
    gluOrtho2D(0.0, width, height, 0.0);
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
