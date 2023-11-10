#include "entity/projectile/projectile_spawner.hpp"
#include "util/bind.hpp"

namespace rl
{
    [[nodiscard]]
    Projectile* ProjectileSpawner::spawn_projectile()
    {
        auto elapsed{ clock_t::now() - m_prev_spawn_time };
        if (elapsed < m_spawn_delay)
            return nullptr;
        else
        {
            Projectile* projectile{ m_scene.instantiate() };
            m_prev_spawn_time = clock_t::now();
            return projectile;
        }
    }

    [[property]]
    double ProjectileSpawner::get_fire_rate() const
    {
        return m_fire_rate;
    }

    [[property]]
    void ProjectileSpawner::set_fire_rate(double fire_rate)
    {
        m_fire_rate = fire_rate;
        m_spawn_delay = ProjectileSpawner::calculate_spawn_delay(m_fire_rate);
    }

    ProjectileSpawner::millisec_t ProjectileSpawner::calculate_spawn_delay(double fire_rate)
    {
        // converts fire rate (shots per second) to the time delay between shots in ms.
        // the multiplication by 100 is just to offset the rounding errors by shifting
        // the decimal place to the right a few places before dividing.
        return (1000ms * 100) / static_cast<uint64_t>(fire_rate * 100);
    }

    void ProjectileSpawner::_bind_methods()
    {
        bind_property(ProjectileSpawner, fire_rate, double);
    }
}
