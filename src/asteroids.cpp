#include "asteroids.h"
#include "obj.h"
#include "world.h"

Asteroids::Asteroids()
    : asteroid(obj::load("models/asteroid.obj", "models/asteroid.mtl"))
{
}

void Asteroids::simulate(const World& world)
{
    for (auto& asteroid : asteroids) {
        asteroid.simulate(world);
    }
}

void Asteroids::apply_diff(const World& world, const Diff& diff)
{
    constexpr auto predicate = [](const auto& asteroid) { return asteroid.should_die; };
    asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(), predicate), asteroids.end());

    last_wave += world.delta;
    if (last_wave >= parameters::asteroid::wave_interval) {
        for (int i = 0; i < wave_num; ++i) {
            asteroids.emplace_back(asteroid, world);
        }
        ++wave_num;
        last_wave = 0;
    }
}

void Asteroids::draw() const
{
    for (const auto& asteroid : asteroids) {
        asteroid.draw();
    }
}

Asteroid::Asteroid(Object asteroid, const World& world)
    : asteroid(asteroid)
{
    constexpr auto rand = []() {
        return rand_range(-(parameters::arena::size * 0.5), parameters::arena::size * 0.5);
    };

    this->asteroid.pos = { rand(), rand(), rand() };
    auto rand_component = static_cast<size_t>(rand_range(0, 2));
    bool rand_snap = rand_range(0, 1);
    this->asteroid.pos[rand_component] = rand_snap ? -(parameters::arena::size * 0.5) : parameters::arena::size * 0.5;

    this->asteroid.z = (world.ship.ship.pos - this->asteroid.pos).norm();

    this->asteroid.scale(rand_range(parameters::asteroid::min_size, parameters::asteroid::max_size));

    speed = rand_range(parameters::asteroid::min_speed, parameters::asteroid::max_speed);
}

void Asteroid::simulate(const World& world)
{
    double z_delta = speed * world.delta;
    asteroid.move({ 0, 0, z_delta });

    auto out_of_bounds = [](auto component) {
        double bound = parameters::arena::size * 0.5;
        return component > bound || component < -bound;
    };

    Vec pos = asteroid.pos;
    if (out_of_bounds(pos.x) || out_of_bounds(pos.y) || out_of_bounds(pos.z)) {
        should_die = true;
    }
}

void Asteroid::draw() const
{
    asteroid.draw();
}
