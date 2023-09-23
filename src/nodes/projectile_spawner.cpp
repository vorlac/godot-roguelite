#include "nodes/projectile_spawner.hpp"

namespace rl::inline node
{
    void ProjectileSpawner::_ready()
    {
        m_scene.set_path(path::scene::Bullet);
    }

    [[nodiscard]]
    Projectile* ProjectileSpawner::spawn_projectile()
    {
        Projectile* projectile{ m_scene.instantiate() };
        return projectile;
    }
}
