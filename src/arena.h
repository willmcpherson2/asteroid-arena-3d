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

    Model top_wall;
    Model bottom_wall;
    Model left_wall;
    Model right_wall;
    Model front_wall;
    Model back_wall;
};

#endif
