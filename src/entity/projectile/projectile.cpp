#include <godot_cpp/variant/vector2.hpp>

#include "entity/projectile/projectile.hpp"
#include "util/engine.hpp"

namespace rl
{
    void Projectile::_ready()
    {
        m_start_pos = this->get_global_position();
        auto forward{ this->get_transform()[0].normalized() };
        this->apply_impulse(forward * m_velocity);
    }

    void Projectile::_process(double delta_time)
    {
        if (engine::editor_active())
            return;

        m_time_to_live -= delta_time;
        if (m_time_to_live <= 0)
        {
            this->queue_free();
            return;
        }

        godot::Vector2 curr_pos{ this->get_global_position() };
        double dist_traveled{ m_start_pos.distance_squared_to(curr_pos) };
        if (dist_traveled >= m_max_travel_dist)
        {
            this->queue_free();
            return;
        }
    }

    [[property]]
    double Projectile::get_movement_speed() const
    {
        return m_movement_speed;
    }

    [[property]]
    double Projectile::get_time_to_live() const
    {
        return m_time_to_live;
    }

    [[property]]
    double Projectile::get_acceleration() const
    {
        return m_acceleration;
    }

    [[property]]
    double Projectile::get_max_travel_dist() const
    {
        return godot::Math::sqrt(m_max_travel_dist);
    }

    [[property]]
    double Projectile::get_velocity() const
    {
        return m_velocity;
    }

    [[property]]
    void Projectile::set_movement_speed(double speed)
    {
        m_movement_speed = speed;
    }

    [[property]]
    void Projectile::set_time_to_live(double ttl)
    {
        m_time_to_live = ttl;
    }

    [[property]]
    void Projectile::set_acceleration(double acceleration)
    {
        m_acceleration = acceleration;
    }

    [[property]]
    void Projectile::set_max_travel_dist(double dist)
    {
        m_max_travel_dist = dist * dist;
    }

    [[property]]
    void Projectile::set_velocity(double velocity)
    {
        m_velocity = velocity;
    }
}
