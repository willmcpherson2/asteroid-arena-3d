#include "world.h"
#include "parameters.h"
#include <GLUT/glut.h>

static void screen_coordinates()
{
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glViewport(0, 0, width, height);
}

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
}

void World::simulate(int delta, Input input)
{
    this->delta = delta;
    this->input = input;

    arena.simulate(*this);
    ship.simulate(*this);
}

void World::set_lighting() const
{
    float pos[] { 0, 20, 0, 0 };
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    float ambient[] { 0, 0, 0, 1 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    float diffuse[] { 1, 1, 1, 1 };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    float specular[] { 1, 1, 1, 1 };
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    float model[] { 0.2f, 0.2f, 0.2f, 1 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    float specular_material[] { 1, 1, 1, 1 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material);

    float emission_material[] { 0, 0, 0, 1 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission_material);
}

void World::draw() const
{
    world_coordinates();
    ship.draw();
    arena.draw();
}
