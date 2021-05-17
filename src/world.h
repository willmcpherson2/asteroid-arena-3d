#ifndef WORLD_H
#define WORLD_H

#include "arena.h"
#include "input.h"
#include "ship.h"

class World {
public:
    void draw() const;
    void simulate(int delta, Input input);

private:
    static void screen_coordinates();
    static void world_coordinates();

    int m_delta { 0 };
    Input m_input;

    Ship m_ship;
    Arena m_arena;
};

#endif
