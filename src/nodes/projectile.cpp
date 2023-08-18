#include "nodes/projectile.hpp"

namespace rl
{
    Projectile::Projectile()
    {
        this->set_name("Projectile");
    }

    Projectile::~Projectile()
    {
        if (!this->is_queued_for_deletion())
            this->queue_free();
    }

    void Projectile::_bind_methods()
    {
    }

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

    [[node_property]]
    double Projectile::get_movement_speed() const
    {
        return m_movement_speed;
    }

    [[node_property]]
    double Projectile::get_time_to_live() const
    {
        return m_time_to_live;
    }

    [[node_property]]
    double Projectile::get_max_travel_dist() const
    {
        return math::sqrt(m_max_travel_dist);
    }

    [[node_property]]
    godot::Vector2 Projectile::get_velocity() const
    {
        return m_velocity;
    }

    [[node_property]]
    void Projectile::set_movement_speed(const double speed)
    {
        m_movement_speed = speed;
    }

    [[node_property]]
    void Projectile::set_time_to_live(const double ttl)
    {
        m_time_to_live = ttl;
    }

    [[node_property]]
    void Projectile::set_max_travel_dist(const double dist)
    {
        m_max_travel_dist = { dist * dist };
    }

    [[node_property]]
    void Projectile::set_velocity(const godot::Vector2 velocity)
    {
        m_velocity = velocity;
    }
}
