#ifndef LIGHT_H
#define LIGHT_H

struct Light {
    void draw() const;
    void draw_ambient() const;
    void draw_directional() const;
};

#endif
