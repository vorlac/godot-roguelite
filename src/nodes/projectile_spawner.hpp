#pragma once

#include "nodes/projectile.hpp"
#include "util/constants.hpp"
#include "util/scene.hpp"

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/typed_array.hpp>

namespace rl::inline node
{
    class ProjectileSpawner : public godot::Node2D
    {
        GDCLASS(ProjectileSpawner, godot::Node2D);

    public:
        ProjectileSpawner() = default;
        virtual ~ProjectileSpawner() = default;

        Projectile* spawn_projectile();

    protected:
        static void _bind_methods()
        {
        }

    protected:
        resource::preload::scene<Projectile> m_scene{ path::scene::Bullet };
    };
}
