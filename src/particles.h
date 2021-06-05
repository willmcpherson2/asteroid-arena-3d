#ifndef PARTICLES_H
#define PARTICLES_H

#include "object.h"
#include <vector>

struct World;
struct Diff;

struct Particle {
    Particle(Object model, Vec pos);
    void simulate(const World& world);
    void draw() const;

    Object particle;
    Vec z;
    int time { 0 };
};

struct Particles {
    Particles();
    void simulate(const World& world);
    void update(const Diff& diff);
    void draw() const;

    Object model;
    std::vector<Particle> particles;
};

#endif
