#include "ship.h"
#include "obj.h"
#include "parameters.h"
#include "world.h"

Ship::Ship()
    : ship(obj::load("models/point-cube.obj"))
{
}

void Ship::simulate(const World& world)
{
    double x_delta = world.delta * input_mouse_sensitivity * world.input.mouse_delta_x();
    double y_delta = world.delta * input_mouse_sensitivity * world.input.mouse_delta_y();
    ship.look(x_delta, y_delta);

    if (world.input.forward()) {
        double forward_delta = ship_speed * world.delta;
        ship.forward(forward_delta);
    }
}

void Ship::draw() const
{
    Object camera = ship;
    camera.forward(-4);
    camera.draw_camera(ship.pos);

    ship.draw(DrawType::Triangles, { 1, 1, 1 });
}
