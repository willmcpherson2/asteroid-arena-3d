#ifndef ARENA_H
#define ARENA_H

#include "input.h"
#include "object.h"

class Arena {
public:
    Arena();
    void draw() const;
    void simulate(int delta, Input input);

private:
    static Object make_wall(double x_theta, double y_theta);

    Object m_top;
    Object m_bottom;
    Object m_left;
    Object m_right;
    Object m_front;
    Object m_back;
};

#endif
