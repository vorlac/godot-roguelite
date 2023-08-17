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

    void Projectile::_ready()
    {
        m_velocity = m_velocity.rotated(this->get_global_rotation());
    }

    void Projectile::_process(double delta_time)
    {
        godot::Vector2 position{ this->get_position() };
        position += m_velocity * delta_time;
        this->set_position(position);
    }
}
