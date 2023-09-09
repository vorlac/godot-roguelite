#include "nodes/projectile_spawner.hpp"

namespace rl::inline node
{
    [[nodiscard]]
    Projectile* ProjectileSpawner::spawn_projectile()
    {
        Projectile* projectile{ m_scene.instantiate() };
        return projectile;
    }
}
