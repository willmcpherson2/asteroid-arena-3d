#ifndef ARENA_H
#define ARENA_H

#include "input.h"
#include "object.h"
#include "parameters.h"

struct World;

struct Arena {
    Arena();
    void draw() const;
    void simulate(const World& world);

    Object arena;
};

#endif
