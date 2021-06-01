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

void Arena::simulate(const World& world)
{
    Vec pos = world.ship.ship.pos;
    constexpr double bound = (parameters::arena::size - parameters::arena::warning_distance) * 0.5;
}

void Arena::draw() const
{
    arena.draw();
}
