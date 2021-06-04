#ifndef WORLD_H
#define WORLD_H

#include "arena.h"
#include "asteroids.h"
#include "bullets.h"
#include "input.h"
#include "light.h"
#include "ship.h"

struct Diff {
    bool reset_world { false };
    bool fire { false };
};

struct World {
    void draw() const;
    void simulate(int delta, Input input);
    void set_lighting() const;

    int delta { 0 };
    Input input;

    Light light;
    Ship ship;
    Arena arena;
    Asteroids asteroids;
    Bullets bullets;
};

#endif
