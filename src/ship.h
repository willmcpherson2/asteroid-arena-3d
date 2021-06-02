#ifndef SHIP_H
#define SHIP_H

#include "input.h"
#include "object.h"

struct World;
struct Diff;

struct Ship {
    Ship();
    void draw() const;
    void simulate(const World& world, Diff& diff);

    Object ship;
    int last_fire;
};

#endif
