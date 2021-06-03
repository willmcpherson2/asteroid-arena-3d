#include "arena.h"
#include "obj.h"
#include "parameters.h"
#include "world.h"

Arena::Arena()
{
    constexpr auto load_wall = [](auto& wall, const auto& obj, const auto& mtl) {
        wall = Object(obj::load(obj, mtl));
        wall.scale(parameters::arena::size);
        wall.set_display(Display::Outline);
    };

    load_wall(grey_front, "models/arena-grey-front.obj", "models/arena-grey-front.mtl");
    load_wall(grey_back, "models/arena-grey-back.obj", "models/arena-grey-back.mtl");
    load_wall(grey_top, "models/arena-grey-top.obj", "models/arena-grey-top.mtl");
    load_wall(grey_bottom, "models/arena-grey-bottom.obj", "models/arena-grey-bottom.mtl");
    load_wall(grey_right, "models/arena-grey-right.obj", "models/arena-grey-right.mtl");
    load_wall(grey_left, "models/arena-grey-left.obj", "models/arena-grey-left.mtl");

    load_wall(red_front, "models/arena-red-front.obj", "models/arena-red-front.mtl");
    load_wall(red_back, "models/arena-red-back.obj", "models/arena-red-back.mtl");
    load_wall(red_top, "models/arena-red-top.obj", "models/arena-red-top.mtl");
    load_wall(red_bottom, "models/arena-red-bottom.obj", "models/arena-red-bottom.mtl");
    load_wall(red_right, "models/arena-red-right.obj", "models/arena-red-right.mtl");
    load_wall(red_left, "models/arena-red-left.obj", "models/arena-red-left.mtl");
}

void Arena::simulate(const World& world, Diff& diff)
{
    Vec pos = world.ship.ship.pos;
    double bound = parameters::arena::warning_size * 0.5;

    warn_front = pos.z > bound;
    warn_back = pos.z < -bound;
    warn_top = pos.y > bound;
    warn_bottom = pos.y < -bound;
    warn_right = pos.x > bound;
    warn_left = pos.x < -bound;
}

void Arena::draw() const
{
    warn_front ? red_front.draw() : grey_front.draw();
    warn_back ? red_back.draw() : grey_back.draw();
    warn_top ? red_top.draw() : grey_top.draw();
    warn_bottom ? red_bottom.draw() : grey_bottom.draw();
    warn_right ? red_right.draw() : grey_right.draw();
    warn_left ? red_left.draw() : grey_left.draw();
}
