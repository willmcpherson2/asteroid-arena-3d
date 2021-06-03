#ifndef ARENA_H
#define ARENA_H

#include "input.h"
#include "object.h"
#include "parameters.h"

struct World;
struct Diff;

struct Arena {
    Arena();
    void draw() const;
    void simulate(const World& world, Diff& diff);

    bool warn_front { false };
    bool warn_back { false };
    bool warn_top { false };
    bool warn_bottom { false };
    bool warn_right { false };
    bool warn_left { false };

    Object grey_front;
    Object grey_back;
    Object grey_top;
    Object grey_bottom;
    Object grey_right;
    Object grey_left;

    Object red_front;
    Object red_back;
    Object red_top;
    Object red_bottom;
    Object red_right;
    Object red_left;
};

#endif
