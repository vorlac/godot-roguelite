#pragma once

#include <godot_cpp/classes/area2d.hpp>
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

    protected:
        static void _bind_methods()
        {
        }

    protected:
        godot::Vector2 m_velocity{ 100.0, 100.0 };
    };

}
