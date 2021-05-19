#include "arena.h"
#include "parameters.h"
#include "world.h"
#include <cmath>

static Model make_wall(double x_theta, double y_theta)
{
    Model wall;

    constexpr int grid_square_size = parameters::arena::size / parameters::arena::grid_divisions;

    for (int int_i = 0; int_i <= parameters::arena::size; int_i += grid_square_size) {
        double i = static_cast<double>(int_i);

        constexpr double offset = static_cast<double>(parameters::arena::size) * 0.5;

        Vec top = (Vec { i, 0, 0 } - offset).rotate_x(x_theta).rotate_y(y_theta);
        Vec bottom = (Vec { i, parameters::arena::size, 0 } - offset).rotate_x(x_theta).rotate_y(y_theta);
        Vec left = (Vec { 0, i, 0 } - offset).rotate_x(x_theta).rotate_y(y_theta);
        Vec right = (Vec { parameters::arena::size, i, 0 } - offset).rotate_x(x_theta).rotate_y(y_theta);

        Line top_bottom;
        top_bottom.add(top);
        top_bottom.add(bottom);
        wall.add(top_bottom);

        Line left_right;
        left_right.add(left);
        left_right.add(right);
        wall.add(left_right);
    }

    return wall;
}

Arena::Arena()
    : top_wall(make_wall(270, 0))
    , bottom_wall(make_wall(90, 0))
    , left_wall(make_wall(0, 270))
    , right_wall(make_wall(0, 90))
    , front_wall(make_wall(0, 180))
    , back_wall(make_wall(0, 0))
{
}

void Arena::simulate(const World& world)
{
    Vec pos = world.ship.ship.pos;
    constexpr double bound = (parameters::arena::size - parameters::arena::warning_distance) * 0.5;

    top_wall.set_colour(pos.y >= bound ? parameters::arena::warning_colour : parameters::arena::colour);
    bottom_wall.set_colour(pos.y <= -bound ? parameters::arena::warning_colour : parameters::arena::colour);
    left_wall.set_colour(pos.x <= -bound ? parameters::arena::warning_colour : parameters::arena::colour);
    right_wall.set_colour(pos.x >= bound ? parameters::arena::warning_colour : parameters::arena::colour);
    front_wall.set_colour(pos.z >= bound ? parameters::arena::warning_colour : parameters::arena::colour);
    back_wall.set_colour(pos.z <= -bound ? parameters::arena::warning_colour : parameters::arena::colour);
}

void Arena::draw() const
{
    top_wall.draw();
    bottom_wall.draw();
    left_wall.draw();
    right_wall.draw();
    front_wall.draw();
    back_wall.draw();
}
