#include "world.h"
#include "obj.h"
#include "parameters.h"
#include <GLUT/glut.h>

static void world_coordinates()
{
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    double width_ratio = static_cast<double>(width) / static_cast<double>(height);
    double height_ratio = static_cast<double>(height) / static_cast<double>(width);
    double ratio = std::max(width_ratio, height_ratio);

    int width_diff = width - height;
    int height_diff = height - width;
    int diff = std::max(width_diff, height_diff);

    int view_width = static_cast<int>(width >= height ? width : width * ratio) - diff;
    int view_height = static_cast<int>(width >= height ? height * ratio : height) - diff;

    int view_x = width_diff > 0 ? width_diff / 2 : 0;
    int view_y = height_diff > 0 ? height_diff / 2 : 0;

    glViewport(view_x, view_y, view_width, view_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(parameters::view::fov, parameters::view::ratio, parameters::view::near_plane, parameters::view::distance);
    glMatrixMode(GL_MODELVIEW);
}

void World::simulate(int delta, Input input)
{
    this->delta = delta;
    this->input = input;

    Diff diff;

    arena.simulate(*this);
    ship.simulate(*this, diff);
    asteroids.simulate(*this, diff);
    particles.simulate(*this);
    bullets.simulate(*this);

    bullets.update(*this, diff);
    particles.update(diff);

    if (diff.reset_world) {
        light = Light();
        ship = Ship();
        arena = Arena();
        asteroids = Asteroids();
        particles = Particles();
        bullets = Bullets();
    }
}

void World::draw() const
{
    world_coordinates();
    light.draw();
    ship.draw();
    arena.draw();
    asteroids.draw();
    particles.draw();
    bullets.draw();
}
