#pragma once

#include <chrono>

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/typed_array.hpp>

#include "core/constants.hpp"
#include "entity/projectile/projectile.hpp"
#include "util/bind.hpp"
#include "util/scene.hpp"

namespace rl
{
    using namespace std::chrono_literals;

    class ProjectileSpawner : public godot::Node2D
    {
        GDCLASS(ProjectileSpawner, godot::Node2D);

    public:
        ProjectileSpawner() = default;
        ~ProjectileSpawner() = default;

        Projectile* spawn_projectile();

    protected:
        [[property]] double get_fire_rate() const;
        [[property]] void set_fire_rate(double fire_rate);

        static void _bind_methods();

    private:
        using clock_t = std::chrono::high_resolution_clock;
        using millisec_t = std::chrono::milliseconds;
        static millisec_t calculate_spawn_delay(double fire_rate);

    private:
        // number of prjectiles per second
        double m_fire_rate{ 10.0 };
        // time delay between shots (ms). multiplication by 100 is just to offset rounding errors.
        millisec_t m_spawn_delay{ ProjectileSpawner::calculate_spawn_delay(m_fire_rate) };
        // the time point that keeps track of when the last projectile was spawned.
        clock_t::time_point m_prev_spawn_time{ clock_t::now() };
        // preloaded packed scene that will be instantiated per spawn
        resource::preload::scene<Projectile> m_scene{ path::scene::Bullet };
    };
}
