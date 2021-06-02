#include "arena.h"
#include "obj.h"
#include "parameters.h"
#include "world.h"

Arena::Arena()
{
    arena = Object(obj::load("models/arena.obj", "models/arena.mtl"));
    arena.scale(parameters::arena::size);
    arena.set_display(Display::Outline);
}

void Arena::simulate(const World& world, Diff& diff)
{
    auto out_of_bounds = [](auto component) {
        double bound = parameters::arena::size * 0.5;
        return component >= bound || component <= -bound;
    };

    Vec pos = world.ship.ship.pos;
    if (out_of_bounds(pos.x) || out_of_bounds(pos.y) || out_of_bounds(pos.z)) {
        diff.reset_world = true;
    }
}

void Arena::draw() const
{
    arena.draw();
}
