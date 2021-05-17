#ifndef SHIP_H
#define SHIP_H

#include "input.h"
#include "object.h"

class Ship {
public:
    Ship();
    void draw() const;
    void simulate(int delta, Input input);

private:
    Object m_ship;
};

#endif
