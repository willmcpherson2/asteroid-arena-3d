#include "ship.h"
#include "obj.h"
#include "parameters.h"

Ship::Ship()
    : m_ship(Obj::load("models/point-cube.obj"))
{
}

void Ship::simulate(int delta, Input input)
{
    double x_delta = delta * input_mouse_sensitivity * input.mouse_delta_x();
    double y_delta = delta * input_mouse_sensitivity * input.mouse_delta_y();
    m_ship.look(x_delta, y_delta);

    if (input.forward()) {
        double forward_delta = ship_speed * delta;
        m_ship.forward(forward_delta);
    }
}

void Ship::draw() const
{
    Object camera = m_ship;
    camera.forward(-4);
    camera.draw_camera(m_ship.pos);

    m_ship.draw(DrawType::Triangles, { 1, 1, 1 });
}
