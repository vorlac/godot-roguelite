#include "nodes/projectile.hpp"

#include <godot_cpp/variant/vector2.hpp>

namespace rl::inline node
{
    void Projectile::_ready()
    {
        m_start_pos = this->get_global_position();
    }

    void Projectile::_process(double delta_time)
    {
        godot::Vector2 position{ this->get_position() };
        position += m_velocity * m_movement_speed * delta_time;
        this->set_position(position);

        // check time to live
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
    double Projectile::get_max_travel_dist() const
    {
        return godot::Math::sqrt(m_max_travel_dist);
    }

    [[property]]
    godot::Vector2 Projectile::get_velocity() const
    {
        return m_velocity;
    }

    [[property]]
    void Projectile::set_movement_speed(const double speed)
    {
        m_movement_speed = speed;
    }

    [[property]]
    void Projectile::set_time_to_live(const double ttl)
    {
        m_time_to_live = ttl;
    }

    [[property]]
    void Projectile::set_max_travel_dist(const double dist)
    {
        m_max_travel_dist = { dist * dist };
    }

    [[property]]
    void Projectile::set_velocity(const godot::Vector2& velocity)
    {
        m_velocity = velocity;
    }
}
