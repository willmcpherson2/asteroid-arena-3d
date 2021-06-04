#include "bullets.h"
#include "obj.h"
#include "world.h"

Bullets::Bullets()
    : bullet(obj::load("models/bullet.obj", "models/bullet.mtl", "models/bullet.data", 64, 64))
{
    bullet.scale(parameters::bullet::size);
    bullet.set_display(Display::Texture);
}

void Bullets::simulate(const World& world)
{
    constexpr auto predicate = [](const auto& bullet) { return bullet.should_die; };
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), predicate), bullets.end());

    for (auto& bullet : bullets) {
        bullet.simulate(world);
    }
}

void Bullets::update(const World& world, const Diff& diff)
{
    if (diff.fire) {
        bullets.emplace_back(bullet, world);
    }
}

void Bullets::draw() const
{
    for (const auto& bullet : bullets) {
        bullet.draw();
    }
}

Bullet::Bullet(Object bullet, const World& world)
    : bullet(bullet)
{
    this->bullet.pos = world.ship.ship.pos;
    z = world.ship.ship.z;

    this->bullet.z = z;
    this->bullet.move({ 0, 0, parameters::ship::size * 0.5 });
    this->bullet.z = world.ship.camera.z;
}

void Bullet::simulate(const World& world)
{
    double z_delta = parameters::bullet::speed * world.delta;
    bullet.z = z;
    bullet.move({ 0, 0, z_delta });
    bullet.z = world.ship.camera.z;

    constexpr auto out_of_bounds = [](auto component) {
        double bound = parameters::arena::size * 0.5;
        return component >= bound || component <= -bound;
    };

    Vec pos = bullet.pos;
    if (out_of_bounds(pos.x) || out_of_bounds(pos.y) || out_of_bounds(pos.z)) {
        should_die = true;
    }
}

void Bullet::draw() const
{
    bullet.draw();
}
