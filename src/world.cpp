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

    int view_width = static_cast<int>(width >= height ? width : width * ratio);
    int view_height = static_cast<int>(width >= height ? height * ratio : height);

    int view_x = 0;
    int view_y = 0;

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

    arena.simulate(*this);
    ship.simulate(*this);
}

void World::draw() const
{
    world_coordinates();
    light.draw();
    ship.draw();
    arena.draw();
}
