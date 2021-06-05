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

    load_wall(grey_front, "assets/arena-grey-front.obj", "assets/arena-grey-front.mtl");
    load_wall(grey_back, "assets/arena-grey-back.obj", "assets/arena-grey-back.mtl");
    load_wall(grey_top, "assets/arena-grey-top.obj", "assets/arena-grey-top.mtl");
    load_wall(grey_bottom, "assets/arena-grey-bottom.obj", "assets/arena-grey-bottom.mtl");
    load_wall(grey_right, "assets/arena-grey-right.obj", "assets/arena-grey-right.mtl");
    load_wall(grey_left, "assets/arena-grey-left.obj", "assets/arena-grey-left.mtl");

    load_wall(red_front, "assets/arena-red-front.obj", "assets/arena-red-front.mtl");
    load_wall(red_back, "assets/arena-red-back.obj", "assets/arena-red-back.mtl");
    load_wall(red_top, "assets/arena-red-top.obj", "assets/arena-red-top.mtl");
    load_wall(red_bottom, "assets/arena-red-bottom.obj", "assets/arena-red-bottom.mtl");
    load_wall(red_right, "assets/arena-red-right.obj", "assets/arena-red-right.mtl");
    load_wall(red_left, "assets/arena-red-left.obj", "assets/arena-red-left.mtl");

    skybox = Object(obj::load("assets/skybox.obj", "assets/skybox.mtl", "assets/skybox.data", 512, 512));
    skybox.set_display(Display::Texture);
    skybox.scale(parameters::arena::skybox_size);
}

void Arena::simulate(const World& world)
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
    skybox.draw();

    constexpr auto draw_wall = [](bool warn, const auto& wall) {
        if (warn) {
            wall.draw();
        }
    };

    draw_wall(warn_front, red_front);
    draw_wall(warn_back, red_back);
    draw_wall(warn_top, red_top);
    draw_wall(warn_bottom, red_bottom);
    draw_wall(warn_right, red_right);
    draw_wall(warn_left, red_left);

    draw_wall(!warn_front, grey_front);
    draw_wall(!warn_back, grey_back);
    draw_wall(!warn_top, grey_top);
    draw_wall(!warn_bottom, grey_bottom);
    draw_wall(!warn_right, grey_right);
    draw_wall(!warn_left, grey_left);
}
