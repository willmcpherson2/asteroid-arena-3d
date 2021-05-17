#include "arena.h"
#include "parameters.h"
#include "world.h"
#include <cmath>

static Object make_wall(double x_theta, double y_theta)
{
    std::vector<Vec> wall;

    constexpr int grid_square_size = parameters::arena::size / parameters::arena::grid_divisions;

    for (int int_i = 0; int_i <= parameters::arena::size; int_i += grid_square_size) {
        double i = static_cast<double>(int_i);

        constexpr double offset = static_cast<double>(parameters::arena::size) * 0.5;

        Vec top = (Vec { i, 0, 0 } - offset).rotate_x(x_theta).rotate_y(y_theta);
        Vec bottom = (Vec { i, parameters::arena::size, 0 } - offset).rotate_x(x_theta).rotate_y(y_theta);
        Vec left = (Vec { 0, i, 0 } - offset).rotate_x(x_theta).rotate_y(y_theta);
        Vec right = (Vec { parameters::arena::size, i, 0 } - offset).rotate_x(x_theta).rotate_y(y_theta);

        wall.push_back(top);
        wall.push_back(bottom);
        wall.push_back(left);
        wall.push_back(right);
    }

    return Object(wall);
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

    top_colour = pos.y >= bound ? parameters::arena::warning_colour : parameters::arena::colour;
    bottom_colour = pos.y <= -bound ? parameters::arena::warning_colour : parameters::arena::colour;
    left_colour = pos.x <= -bound ? parameters::arena::warning_colour : parameters::arena::colour;
    right_colour = pos.x >= bound ? parameters::arena::warning_colour : parameters::arena::colour;
    front_colour = pos.z >= bound ? parameters::arena::warning_colour : parameters::arena::colour;
    back_colour = pos.z <= -bound ? parameters::arena::warning_colour : parameters::arena::colour;
}

void Arena::draw() const
{
    DrawType draw_type = DrawType::Lines;

    top_wall.draw(draw_type, top_colour);
    bottom_wall.draw(draw_type, bottom_colour);
    left_wall.draw(draw_type, left_colour);
    right_wall.draw(draw_type, right_colour);
    front_wall.draw(draw_type, front_colour);
    back_wall.draw(draw_type, back_colour);
}
