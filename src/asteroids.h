#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include "object.h"
#include "parameters.h"
#include <vector>

struct World;
struct Diff;

struct Asteroid {
    Asteroid(Object asteroid, const World& world);
    void simulate(const World& world, Diff& diff);
    void draw() const;

    Object asteroid;
    Object pos;
    Object rotation;
    double speed { 0 };
    double rotation_speed { 0 };
    double health { 0 };
    bool in_arena { false };
    bool should_die { false };
};

struct Asteroids {
    Asteroids();
    void simulate(const World& world, Diff& diff);
    void draw() const;

    Object model;
    std::vector<Asteroid> asteroids;
    int last_wave { parameters::asteroid::wave_interval };
    int wave_num { 1 };
};

#endif
