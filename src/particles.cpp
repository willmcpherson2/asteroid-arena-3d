#include "particles.h"
#include "obj.h"
#include "world.h"

Particles::Particles()
    : model(obj::load("assets/particle.obj", "assets/particle.mtl", "assets/particle.data", 32, 32))
{
    model.scale(parameters::particle::size);
}

void Particles::simulate(const World& world)
{
    constexpr auto predicate = [](const auto& particle) { return particle.time > parameters::particle::duration; };
    particles.erase(std::remove_if(particles.begin(), particles.end(), predicate), particles.end());

    auto furthest_first = [&](const Particle& a, const Particle& b) {
        auto a_to_camera = a.particle.pos - world.ship.camera.pos;
        auto b_to_camera = b.particle.pos - world.ship.camera.pos;
        return a_to_camera.length() > b_to_camera.length();
    };
    std::sort(particles.begin(), particles.end(), furthest_first);

    for (auto& particle : particles) {
        particle.simulate(world);
    }
}

void Particles::update(const Diff& diff)
{
    for (const auto& pos : diff.death_positions) {
        for (int i = 0; i < parameters::particle::amount; ++i) {
            particles.emplace_back(model, pos);
        }
    }
}

void Particles::draw() const
{
    for (const auto& particle : particles) {
        particle.draw();
    }
}

Particle::Particle(Object model, Vec pos)
    : particle(model)
{
    particle.pos = pos;
    z = rand_vec(-1, 1).norm();
}

void Particle::simulate(const World& world)
{
    double z_delta = parameters::particle::speed * world.delta;
    particle.z = z;
    particle.move({ 0, 0, z_delta });
    particle.z = (particle.pos - world.ship.camera.pos).norm();

    time += world.delta;

    particle.size = parameters::particle::size * (parameters::particle::duration - time) / parameters::particle::duration;
}

void Particle::draw() const
{
    particle.draw();
}
