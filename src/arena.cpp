#include "arena.h"
#include "parameters.h"

Arena::Arena()
    : m_top(make_wall(0, 270))
    , m_bottom(make_wall(0, 90))
    , m_left(make_wall(270, 0))
    , m_right(make_wall(90, 0))
    , m_front(make_wall(0, 180))
    , m_back(make_wall(0, 0))
{
}

Object Arena::make_wall(double x_theta, double y_theta)
{
    std::vector<Vec> wall;

    constexpr int grid_square_size = arena_size / arena_grid_divisions;

    for (int int_i = 0; int_i <= arena_size; int_i += grid_square_size) {
        double i = static_cast<double>(int_i);

        constexpr double offset = static_cast<double>(arena_size) * 0.5;

        Vec top = (Vec { i, 0, 0 } - offset).rotate_x(x_theta).rotate_y(y_theta);
        Vec bottom = (Vec { i, arena_size, 0 } - offset).rotate_x(x_theta).rotate_y(y_theta);
        Vec left = (Vec { 0, i, 0 } - offset).rotate_x(x_theta).rotate_y(y_theta);
        Vec right = (Vec { arena_size, i, 0 } - offset).rotate_x(x_theta).rotate_y(y_theta);

        wall.push_back(top);
        wall.push_back(bottom);
        wall.push_back(left);
        wall.push_back(right);
    }

    return Object(wall);
}

void Arena::draw() const
{
    DrawType draw_type = DrawType::Lines;
    Colour colour { 0.5, 0.5, 0.5 };

    m_top.draw(draw_type, colour);
    m_bottom.draw(draw_type, colour);
    m_left.draw(draw_type, colour);
    m_right.draw(draw_type, colour);
    m_front.draw(draw_type, colour);
    m_back.draw(draw_type, colour);
}
