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
    double x_delta = world.delta * parameters::input::mouse_sensitivity * world.input.mouse_delta_x();
    double y_delta = world.delta * parameters::input::mouse_sensitivity * world.input.mouse_delta_y();
    ship.look(x_delta, y_delta);

    if (world.input.forward()) {
        double z_delta = parameters::ship::speed * world.delta;
        ship.move({ 0, 0, z_delta });
    }

    last_fire += world.delta;
    if (world.input.fire() && last_fire >= parameters::ship::fire_rate) {
        diff.fire = true;
        last_fire = 0;
    }
}

void Ship::draw() const
{
    Object camera = ship;
    camera.move(parameters::ship::camera_pos);
    Vec focus = ship.pos + ship.y * parameters::ship::camera_pos.y;
    camera.draw_camera(focus);

    // Object camera;
    // Vec focus = ship.pos;
    // camera.draw_camera(focus);

    ship.draw();
}
