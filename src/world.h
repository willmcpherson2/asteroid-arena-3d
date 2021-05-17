#ifndef WORLD_H
#define WORLD_H

#include "arena.h"
#include "input.h"
#include "ship.h"

struct World {
    void draw() const;
    void simulate(int delta, Input input);

    int delta { 0 };
    Input input;

    Ship ship;
    Arena arena;
};

#endif
