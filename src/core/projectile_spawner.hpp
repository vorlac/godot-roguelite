#pragma once

#include "nodes/projectile.hpp"
#include "util/constants.hpp"
#include "util/scene.hpp"

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/wrapped.hpp>

namespace rl
{
    class ProjectileSpawner : public godot::Node2D
    {
        GDCLASS(ProjectileSpawner, godot::Node2D);

    public:
        ProjectileSpawner();
        virtual ~ProjectileSpawner();

        rl::Projectile* spawn_projectile();

    protected:
        static void _bind_methods()
        {
        }

    protected:
        resource::preload::scene<rl::Projectile> m_scene{ path::scene::Bullet };
    };
}
