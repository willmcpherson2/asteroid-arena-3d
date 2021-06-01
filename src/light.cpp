#include "light.h"
#include "parameters.h"
#include <OpenGL/gl.h>

void Light::draw() const
{
    draw_ambient();
    draw_directional();
}

void Light::draw_ambient() const
{
    float ambient[] = { 0.5, 0.5, 0.5, 1 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    float diffuse[] = { 0, 0, 0, 1 };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    float specular[] = { 0, 0, 0, 1 };
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

void Light::draw_directional() const
{
    float ambient[] = { 0.5, 0.5, 0.5, 1 };
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);

    float diffuse[] = { 0.5, 0.5, 0.5, 1 };
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);

    float specular[] = { 0.5, 0.5, 0.5, 1 };
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);

    float position[] = { 0, parameters::arena::size, 0, 1 };
    glLightfv(GL_LIGHT1, GL_POSITION, position);

    float spot_direction[] = { 0, -1, 0 };
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);

    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45);
}
