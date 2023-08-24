#pragma once

#include "nodes/projectile.hpp"
#include "util/utils.hpp"

#include <utility>
#include <vector>

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace rl
{
    class ProjectileSpawner : public godot::Node2D
    {
        GDCLASS(ProjectileSpawner, godot::Node2D);

    public:
        ProjectileSpawner();
        virtual ~ProjectileSpawner();

        [[nodiscard]]
        rl::Projectile* const spawn_projectile();


    protected:
        static void _bind_methods()
        {
        }

    protected:
        resource::preload::scene<rl::Projectile> m_scene{ path::scene::Bullet };
    };
}