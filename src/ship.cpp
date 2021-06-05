#include "ship.h"
#include "obj.h"
#include "parameters.h"
#include "world.h"

Ship::Ship()
    : ship(obj::load("assets/ship.obj", "assets/ship.mtl"))
{
    ship.scale(parameters::ship::size);
}

void Ship::simulate(const World& world, Diff& diff)
{
    if (dead) {
        if (world.input.any()) {
            diff.reset_world = true;
        }
        return;
    }

    auto out_of_bounds = [](auto component) {
        double bound = parameters::arena::size * 0.5;
        return component >= bound || component <= -bound;
    };

    Vec pos = ship.pos;
    if (out_of_bounds(pos.x) || out_of_bounds(pos.y) || out_of_bounds(pos.z)) {
        diff.death_positions.push_back(pos);
        dead = true;
        return;
    }

    for (const auto& asteroid : world.asteroids.asteroids) {
        double distance = (ship.pos - asteroid.asteroid.pos).length();
        if (distance - asteroid.asteroid.size.x * 0.5 - parameters::ship::size * 0.5 < 0) {
            diff.death_positions.push_back(pos);
            dead = true;
            return;
        }
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

    auto camera_pos = [&]() {
        if (world.input.left()) {
            draw_ship = false;
            return parameters::ship::left_camera_pos;
        } else if (world.input.right()) {
            draw_ship = false;
            return parameters::ship::right_camera_pos;
        } else if (world.input.back()) {
            draw_ship = false;
            return parameters::ship::back_camera_pos;
        } else {
            draw_ship = true;
            return parameters::ship::forward_camera_pos;
        }
    }();
    camera = ship;
    camera.move(camera_pos);
    focus = ship.pos + ship.y * camera_pos.y;
    camera.draw_camera(focus);
}

void Ship::draw() const
{
    camera.draw_camera(focus);
    if (!dead && draw_ship) {
        ship.draw();
    }
}
