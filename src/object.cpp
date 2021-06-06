#include "object.h"
#include "parameters.h"
#include <GLUT/glut.h>

Object::Object(Model model)
    : model(model)
{
}

void Object::draw_camera(Vec focus) const
{
    glMatrixMode(GL_PROJECTION);
    gluLookAt(pos.x, pos.y, pos.z, focus.x, focus.y, focus.z, y.x, y.y, y.z);
    glMatrixMode(GL_MODELVIEW);
}

void Object::draw() const
{
    glPushMatrix();

    glTranslated(pos.x, pos.y, pos.z);

    Vec tilt_axis = z.flatten_y().cross(z);
    double tilt_rotation = z.flatten_y().theta(z);
    glRotated(tilt_rotation, tilt_axis.x, tilt_axis.y, tilt_axis.z);

    double spin_theta = z.flatten_y().theta({ 0, 0, 1 });
    double spin_rotation = z.x >= 0 ? spin_theta : -spin_theta;
    glRotated(spin_rotation, 0, 1, 0);

    double flip_rotation = y.y >= 0 ? 0 : 180;
    glRotated(flip_rotation, 1, 0, 0);
    glRotated(flip_rotation, 0, 1, 0);

    glScaled(size.x, size.y, size.z);

    model.draw();

    glPopMatrix();
}

void Object::move(Vec pos)
{
    this->pos = this->pos + x * pos.x;
    this->pos = this->pos + y * pos.y;
    this->pos = this->pos + z * pos.z;
}

void Object::scale(double size)
{
    this->size = this->size * size;
}

void Object::scale(Vec size)
{
    this->size = this->size * size;
}

void Object::rotate(Vec axis, double theta)
{
    x = x.rotate(axis, theta);
    y = y.rotate(axis, theta);
    z = z.rotate(axis, theta);
}

void Object::rotate_x(double theta)
{
    x = x.rotate_x(theta);
    y = y.rotate_x(theta);
    z = z.rotate_x(theta);
}

void Object::rotate_y(double theta)
{
    x = x.rotate_y(theta);
    y = y.rotate_y(theta);
    z = z.rotate_y(theta);
}

void Object::rotate_z(double theta)
{
    x = x.rotate_z(theta);
    y = y.rotate_z(theta);
    z = z.rotate_z(theta);
}

void Object::look(double x_delta, double y_delta)
{
    bool up_vector_top_hemisphere = y.theta({ 0, 1, 0 }) < 90;
    x_delta = up_vector_top_hemisphere ? x_delta : -x_delta;

    rotate_y(x_delta);
    rotate(x, y_delta);

    assert(near(x.theta(y), 90));
    assert(near(x.theta(z), 90));
    assert(near(y.theta(z), 90));
}

void Object::set_display(Display display)
{
    for (auto& polygon : model.polygons) {
        polygon.display = display;
        polygon.material.display = display;
    }
}

void Model::draw() const
{
    for (auto polygon : polygons) {
        polygon.draw();
    }
}

void Model::add(Polygon polygon)
{
    polygons.push_back(polygon);
}

void Polygon::add(Vertex vertex)
{
    vertices.push_back(vertex);
}

void Polygon::draw() const
{
    material.draw();

    for (auto vertex : vertices) {
        vertex.draw();
    }

    material.reset();
}

void Vertex::draw() const
{
    glNormal3d(normal.x, normal.y, normal.z);
    glTexCoord2d(uv.x, uv.y);
    glVertex3d(vertex.x, vertex.y, vertex.z);
}

void Material::draw() const
{
    auto material = [&]() {
        float a[] { static_cast<float>(ambient.r), static_cast<float>(ambient.g), static_cast<float>(ambient.b), 1 };
        glMaterialfv(GL_FRONT, GL_AMBIENT, a);

        float d[] { static_cast<float>(diffuse.r), static_cast<float>(diffuse.g), static_cast<float>(diffuse.b), 1 };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, d);

        float s[] { static_cast<float>(specular.r), static_cast<float>(specular.g), static_cast<float>(specular.b), 1 };
        glMaterialfv(GL_FRONT, GL_SPECULAR, s);

        float e[] { static_cast<float>(emissive.r), static_cast<float>(emissive.g), static_cast<float>(emissive.b), 1 };
        glMaterialfv(GL_FRONT, GL_EMISSION, e);
    };

    switch (display) {
        case Display::Outline:
            glDisable(GL_LIGHTING);
            glColor3d(diffuse.r, diffuse.g, diffuse.b);

            glBegin(GL_LINE_LOOP);

            break;
        case Display::Solid:
            material();

            glBegin(GL_POLYGON);

            break;
        case Display::Texture:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture.id);

            material();

            glBegin(GL_POLYGON);

            break;
    }
}

void Material::reset() const
{
    switch (display) {
        case Display::Outline:
            glEnd();
            glEnable(GL_LIGHTING);
            break;
        case Display::Solid:
            glEnd();
            break;
        case Display::Texture:
            glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
            break;
    }
}
