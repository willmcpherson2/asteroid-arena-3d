#include "asteroids.h"
#include "obj.h"
#include "world.h"

Asteroids::Asteroids()
    : model(obj::load("assets/asteroid.obj", "assets/asteroid.mtl", "assets/asteroid.data", 64, 64))
{
}

void Asteroids::simulate(const World& world, Diff& diff)
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
        asteroid.simulate(world, diff);
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

    for (auto& polygon : asteroid.model.polygons) {
        for (auto& vertex : polygon.vertices) {
            auto perturbed_vertex = vertex.vertex + rand_vec(-parameters::asteroid::perturbation, parameters::asteroid::perturbation);

            for (auto& polygon : asteroid.model.polygons) {
                for (auto& other_vertex : polygon.vertices) {
                    if (&vertex != &other_vertex && vertex.vertex.near(other_vertex.vertex)) {
                        other_vertex.vertex = perturbed_vertex;
                    }
                }
            }

            vertex.vertex = perturbed_vertex;
        }
    }
}

void Asteroid::simulate(const World& world, Diff& diff)
{
    double z_delta = speed * world.delta;
    asteroid.move({ 0, 0, z_delta });

    constexpr auto in_bounds = [](double component) {
        return component < parameters::arena::size * 0.5 && component > -(parameters::arena::size * 0.5);
    };

    if (!in_arena) {
        if (in_bounds(asteroid.pos.x) && in_bounds(asteroid.pos.y) && in_bounds(asteroid.pos.z)) {
            in_arena = true;
        } else {
            return;
        }
    }

    auto flip = [&](auto pos_component, auto& z_component) {
        if (!in_bounds(pos_component)) {
            z_component = -z_component;
        }
    };

    flip(asteroid.pos.x, asteroid.z.x);
    flip(asteroid.pos.y, asteroid.z.y);
    flip(asteroid.pos.z, asteroid.z.z);

    constexpr auto adjust = [](auto& component) {
        if (component > parameters::arena::size * 0.5) {
            component = parameters::arena::size * 0.5;
        }
        if (component < -(parameters::arena::size * 0.5)) {
            component = -(parameters::arena::size * 0.5);
        }
    };

    adjust(asteroid.pos.x);
    adjust(asteroid.pos.y);
    adjust(asteroid.pos.z);

    for (const auto& bullet : world.bullets.bullets) {
        double distance = (asteroid.pos - bullet.bullet.pos).length();
        if (distance - asteroid.size.x * 0.5 - parameters::bullet::size * 0.5 < 0) {
            health -= parameters::bullet::damage;
            if (health <= 0) {
                should_die = true;
                diff.death_positions.push_back(asteroid.pos);
                return;
            }
        }
    }
}

void Asteroid::draw() const
{
    asteroid.draw();
}
