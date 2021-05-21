#include "ship.h"
#include "obj.h"
#include "parameters.h"
#include "world.h"

Ship::Ship()
    : ship(obj::load("models/ship.obj"))
{
}

void Ship::simulate(const World& world)
{
    double x_delta = world.delta * parameters::input::mouse_sensitivity * world.input.mouse_delta_x();
    double y_delta = world.delta * parameters::input::mouse_sensitivity * world.input.mouse_delta_y();
    ship.look(x_delta, y_delta);

    if (world.input.forward()) {
        double forward_delta = parameters::ship::speed * world.delta;
        ship.forward(forward_delta);
    }
}

void Ship::draw() const
{
    Object camera = ship;
    camera.forward(parameters::ship::camera_distance);
    camera.draw_camera(ship.pos);

    Object light = ship;
    light.forward(parameters::ship::light_distance);
    light.draw_light();

    ship.draw();
}
