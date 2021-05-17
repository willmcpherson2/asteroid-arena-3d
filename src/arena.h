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

    Object top_wall;
    Object bottom_wall;
    Object left_wall;
    Object right_wall;
    Object front_wall;
    Object back_wall;

    Colour top_colour { arena_colour };
    Colour bottom_colour { arena_colour };
    Colour left_colour { arena_colour };
    Colour right_colour { arena_colour };
    Colour front_colour { arena_colour };
    Colour back_colour { arena_colour };
};

#endif
