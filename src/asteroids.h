#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include "object.h"
#include "parameters.h"
#include <vector>

struct World;
struct Diff;

struct Asteroid {
    Asteroid(Object asteroid, const World& world);
    void simulate(const World& world);
    void draw() const;

    Object asteroid;
    double speed { 0 };
    bool should_die { false };
};

struct Asteroids {
    Asteroids();
    void simulate(const World& world);
    void draw() const;

    Object asteroid;
    std::vector<Asteroid> asteroids;
    int last_wave { parameters::asteroid::wave_interval };
    int wave_num { 1 };
};

#endif
