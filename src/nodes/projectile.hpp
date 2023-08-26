#pragma once

#include "util/bind.hpp"

#include <godot_cpp/classes/area2d.hpp>

// #include <godot_cpp/variant/vector2.hpp>

namespace godot
{
    struct Vector2;
}

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

        static void _bind_methods();

        [[node_property]] double get_movement_speed() const;
        [[node_property]] double get_time_to_live() const;
        [[node_property]] double get_max_travel_dist() const;
        [[node_property]] double get_acceleration() const;
        [[node_property]] godot::Vector2 get_velocity() const;
        [[node_property]] void set_movement_speed(const double speed);
        [[node_property]] void set_time_to_live(const double ttl);
        [[node_property]] void set_max_travel_dist(const double dist);
        [[node_property]] void set_acceleration(const double acceleration);
        [[node_property]] void set_velocity(const godot::Vector2& velocity);

    protected:
        // projectile movement velocity (pixels)
        godot::Vector2 m_velocity{ 1.0, 1.0 };
        // projectile movement speed (pixels/s)
        double m_movement_speed{ 1000.0 };
        // projectile acceleration (pixels/s/s)
        double m_accelleration{ 100.0 };
        // max time duration alive (seconds)
        double m_time_to_live{ 1.0 };
        // max travel distance (pixels) - uint32_t?
        double m_max_travel_dist{ 1000.0 * 1000.0 };

    protected:
        godot::Vector2 m_start_pos{ 0.0, 0.0 };
    };
}
