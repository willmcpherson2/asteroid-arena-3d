#include "asteroids.h"
#include "obj.h"
#include "world.h"

Asteroids::Asteroids()
    : model(obj::load("assets/asteroid.obj", "assets/asteroid.mtl"))
{
}

void Asteroids::simulate(const World& world)
{
    constexpr auto predicate = [](const auto& asteroid) { return asteroid.should_die; };
    asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(), predicate), asteroids.end());

    last_wave += world.delta;
    if (last_wave >= parameters::asteroid::wave_interval) {
        for (int i = 0; i < wave_num; ++i) {
            asteroids.emplace_back(model, world);
        }
        ++wave_num;
        last_wave = 0;
    }

    for (auto& asteroid : asteroids) {
        asteroid.simulate(world);
    }
}

void Asteroids::draw() const
{
    for (const auto& asteroid : asteroids) {
        asteroid.draw();
    }
}

Asteroid::Asteroid(Object model, const World& world)
    : asteroid(model)
{
    constexpr auto rand = []() {
        return rand_range(-(parameters::arena::skybox_size * 0.5), parameters::arena::skybox_size * 0.5);
    };

    asteroid.pos = { rand(), rand(), rand() };
    auto rand_component = static_cast<size_t>(rand_range(0, 2));
    bool rand_snap = rand_range(0, 1);
    asteroid.pos[rand_component] = rand_snap ? -(parameters::arena::skybox_size * 0.5) : parameters::arena::skybox_size * 0.5;

    asteroid.z = (world.ship.ship.pos - asteroid.pos).norm();

    double size = rand_range(parameters::asteroid::min_size, parameters::asteroid::max_size);
    asteroid.scale(size);
    health = size;

    speed = rand_range(parameters::asteroid::min_speed, parameters::asteroid::max_speed);
}

void Asteroid::simulate(const World& world)
{
    double z_delta = speed * world.delta;
    asteroid.move({ 0, 0, z_delta });

    auto out_of_bounds = [](auto component) {
        double bound = parameters::arena::skybox_size * 0.5;
        return component > bound || component < -bound;
    };

    Vec pos = asteroid.pos;
    if (out_of_bounds(pos.x) || out_of_bounds(pos.y) || out_of_bounds(pos.z)) {
        should_die = true;
    }

    for (const auto& bullet : world.bullets.bullets) {
        double distance = (asteroid.pos - bullet.bullet.pos).length();
        if (distance - asteroid.size.x * 0.5 - parameters::bullet::size * 0.5 < 0) {
            health -= parameters::bullet::damage;
            if (health <= 0) {
                should_die = true;
            }
        }
    }
}

void Asteroid::draw() const
{
    asteroid.draw();
}
