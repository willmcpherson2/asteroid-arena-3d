#ifndef SHIP_H
#define SHIP_H

#include "input.h"
#include "object.h"
#include "parameters.h"

struct World;
struct Diff;

struct Ship {
    Ship();
    void draw() const;
    void simulate(const World& world, Diff& diff);

    Object ship;
    bool draw_ship { true };
    Object camera;
    Vec focus;
    int last_fire { parameters::ship::fire_interval };
};

#endif
