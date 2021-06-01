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
        return component >= parameters::arena::size || component <= -parameters::arena::size;
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
