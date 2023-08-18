#pragma once

#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/path2d.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace rl
{
    class Projectile : public godot::Area2D
    {
        GDCLASS(Projectile, godot::Area2D);

    public:
        Projectile();
        virtual ~Projectile();

        void _ready() override;
        void _process(double delta_time) override;

        void _mouse_enter() override
        {
        }

    protected:
        static void _bind_methods()
        {
        }

    protected:
        // projectile speed
        godot::Vector2 m_velocity{ 1000, 1000 };
        // time to live
        double m_ttl{ 1.0 };
        // collision layer mask
        // TODO: figure out how to define collision layer mask
        godot::Vector2 m_scale{ 1.0, 1.0 };
        // tween to use for trajectory behavior
        godot::Ref<godot::Tween> m_tween{};
    };
}
