#include "ship.h"
#include "obj.h"
#include "parameters.h"
#include "world.h"

Ship::Ship()
    : ship(obj::load("models/ship.obj", "models/ship.mtl"))
{
    ship.scale(parameters::ship::size);
}

void Ship::simulate(const World& world, Diff& diff)
{
    auto out_of_bounds = [](auto component) {
        double bound = parameters::arena::size * 0.5;
        return component >= bound || component <= -bound;
    };

    Vec pos = ship.pos;
    if (out_of_bounds(pos.x) || out_of_bounds(pos.y) || out_of_bounds(pos.z)) {
        diff.reset_world = true;
        return;
    }

    double x_delta = world.delta * parameters::input::mouse_sensitivity * world.input.mouse_delta_x();
    double y_delta = world.delta * parameters::input::mouse_sensitivity * world.input.mouse_delta_y();
    ship.look(x_delta, y_delta);

    if (world.input.forward()) {
        double z_delta = parameters::ship::speed * world.delta;
        ship.move({ 0, 0, z_delta });
    }

    last_fire += world.delta;
    if (world.input.fire() && last_fire >= parameters::ship::fire_interval) {
        diff.fire = true;
        last_fire = 0;
    }

    camera = ship;
    camera.move(parameters::ship::camera_pos);
    focus = ship.pos + ship.y * parameters::ship::camera_pos.y;
    camera.draw_camera(focus);
}

void Ship::draw() const
{
    camera.draw_camera(focus);
    ship.draw();
}
