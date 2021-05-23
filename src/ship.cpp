#include "ship.h"
#include "obj.h"
#include "parameters.h"
#include "world.h"

Ship::Ship()
    : ship(obj::load("models/ship.obj", "models/ship.mtl"))
{
}

void Ship::simulate(const World& world)
{
    double x_delta = world.delta * parameters::input::mouse_sensitivity * world.input.mouse_delta_x();
    double y_delta = world.delta * parameters::input::mouse_sensitivity * world.input.mouse_delta_y();
    ship.look(x_delta, y_delta);

    if (world.input.forward()) {
        double z_delta = parameters::ship::speed * world.delta;
        ship.move({ 0, 0, z_delta });
    }
}

void Ship::draw() const
{
    Object camera = ship;
    camera.move(parameters::ship::camera_pos);
    camera.draw_camera(ship.pos);

    Object light = ship;
    light.move(parameters::ship::light_pos);
    light.draw_light();

    ship.draw();
}
