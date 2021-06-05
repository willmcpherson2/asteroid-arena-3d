#include "bullets.h"
#include "obj.h"
#include "world.h"

Bullets::Bullets()
    : model(obj::load("assets/bullet.obj", "assets/bullet.mtl", "assets/bullet.data", 64, 64))
{
    model.scale(parameters::bullet::size);
    model.set_display(Display::Texture);
}

void Bullets::simulate(const World& world)
{
    constexpr auto predicate = [](const auto& bullet) { return bullet.should_die; };
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), predicate), bullets.end());

    auto furthest_first = [&](const Bullet& a, const Bullet& b) {
        auto a_to_camera = a.bullet.pos - world.ship.camera.pos;
        auto b_to_camera = b.bullet.pos - world.ship.camera.pos;
        return a_to_camera.length() > b_to_camera.length();
    };
    std::sort(bullets.begin(), bullets.end(), furthest_first);

    for (auto& bullet : bullets) {
        bullet.simulate(world);
    }
}

void Bullets::update(const World& world, const Diff& diff)
{
    if (diff.fire) {
        bullets.emplace_back(model, world);
    }
}

void Bullets::draw() const
{
    for (const auto& bullet : bullets) {
        bullet.draw();
    }
}

Bullet::Bullet(Object model, const World& world)
    : bullet(model)
{
    bullet.pos = world.ship.ship.pos;
    z = world.ship.ship.z;
    bullet.z = z;
    bullet.move({ 0, 0, parameters::ship::size * 0.5 });
}

void Bullet::simulate(const World& world)
{
    double z_delta = parameters::bullet::speed * world.delta;
    bullet.z = z;
    bullet.move({ 0, 0, z_delta });
    bullet.z = (bullet.pos - world.ship.camera.pos).norm();

    constexpr auto out_of_bounds = [](auto component) {
        double bound = parameters::arena::skybox_size * 0.5;
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
