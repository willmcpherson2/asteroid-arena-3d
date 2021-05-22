#include "light.h"
#include "parameters.h"
#include <OpenGL/gl.h>

void Light::draw() const
{
    float ambient[] = { 1, 1, 1, 1 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    float diffuse[] = { 0, 0, 0, 1 };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    float specular[] = { 0, 0, 0, 1 };
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}
