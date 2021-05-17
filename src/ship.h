#ifndef SHIP_H
#define SHIP_H

#include "input.h"
#include "object.h"

struct World;

struct Ship {
    Ship();
    void draw() const;
    void simulate(const World& world);

    Object ship;
};

#endif
