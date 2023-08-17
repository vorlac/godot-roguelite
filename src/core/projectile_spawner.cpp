#include "core/projectile_spawner.hpp"

#include "nodes/character.hpp"

namespace rl
{
    ProjectileSpawner::ProjectileSpawner()
    {
        this->set_name("Projectile");
        rl::Projectile* test{ m_scene.instantiate() };
        debug::assert(test != nullptr);
    }

    ProjectileSpawner::~ProjectileSpawner()
    {
        if (!this->is_queued_for_deletion())
            this->queue_free();
    }

    [[nodiscard]]
    rl::Projectile* const ProjectileSpawner::spawn_projectile()
    {
        return m_scene.instantiate();
    }

}
