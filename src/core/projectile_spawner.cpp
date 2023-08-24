#include "core/projectile_spawner.hpp"

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
    }

    [[nodiscard]]
    rl::Projectile* const ProjectileSpawner::spawn_projectile()
    {
        return m_scene.instantiate();
    }

}
