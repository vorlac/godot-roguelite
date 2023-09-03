#include "nodes/projectile_spawner.hpp"

namespace rl
{
    ProjectileSpawner::ProjectileSpawner()
    {
        this->set_name("Projectile");
    }

    ProjectileSpawner::~ProjectileSpawner()
    {
    }

    [[nodiscard]]
    rl::Projectile* ProjectileSpawner::spawn_projectile()
    {
        rl::Projectile* projectile{ m_scene.instantiate() };
        return projectile;
    }

}
