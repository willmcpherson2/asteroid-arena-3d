#ifndef BULLETS_H
#define BULLETS_H

#include "object.h"
#include <vector>

struct World;
struct Diff;

struct Bullet {
    Bullet(Object bullet, Vec pos, Vec z);
    void simulate(const World& world);
    void draw() const;

    Object bullet;
    bool should_die { false };
};

struct Bullets {
    Bullets();
    void simulate(const World& world);
    void apply_diff(const World& world, const Diff& diff);
    void draw() const;

    Object bullet;
    std::vector<Bullet> bullets;
};

#endif
