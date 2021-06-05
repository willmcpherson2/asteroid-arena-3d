#ifndef BULLETS_H
#define BULLETS_H

#include "object.h"
#include <vector>

struct World;
struct Diff;

struct Bullet {
    Bullet(Object model, const World& world);
    void simulate(const World& world);
    void draw() const;

    Object bullet;
    Vec z;
    bool should_die { false };
};

struct Bullets {
    Bullets();
    void simulate(const World& world);
    void update(const World& world, const Diff& diff);
    void draw() const;

    Object model;
    std::vector<Bullet> bullets;
};

#endif
