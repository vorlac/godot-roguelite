#pragma once

#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/variant/vector2.hpp>

#include "core/attributes.hpp"
#include "singletons/console.hpp"
#include "util/bind.hpp"
#include "util/conversions.hpp"
#include "util/io.hpp"

namespace rl
{
    class Projectile : public godot::RigidBody2D
    {
        GDCLASS(Projectile, godot::RigidBody2D);

    public:
        Projectile() = default;
        virtual ~Projectile() = default;

        void _ready() override;
        void _process(double delta_time) override;

        [[property]] double get_movement_speed() const;
        [[property]] double get_time_to_live() const;
        [[property]] double get_max_travel_dist() const;
        [[property]] double get_acceleration() const;
        [[property]] double get_velocity() const;

        [[property]] void set_movement_speed(double speed);
        [[property]] void set_time_to_live(double ttl);
        [[property]] void set_max_travel_dist(double dist);
        [[property]] void set_acceleration(double acceleration);
        [[property]] void set_velocity(double velocity);

    protected:
        static void _bind_methods()
        {
            bind_member_function(Projectile, get_movement_speed);
            bind_member_function(Projectile, get_time_to_live);
            bind_member_function(Projectile, get_max_travel_dist);
            bind_member_function(Projectile, get_acceleration);
            bind_member_function(Projectile, get_velocity);
            bind_member_function(Projectile, set_movement_speed);
            bind_member_function(Projectile, set_time_to_live);
            bind_member_function(Projectile, set_max_travel_dist);
            bind_member_function(Projectile, set_acceleration);
            bind_member_function(Projectile, set_velocity);
        }

    protected:
        godot::Vector2 m_start_pos{ 0.0, 0.0 };
        // projectile movement velocity (pixels)
        double m_velocity{ 1500 };
        // projectile movement speed (pixels/s)
        double m_movement_speed{ 1000.0 };
        // projectile acceleration (pixels/s/s)
        double m_acceleration{ 100.0 };
        // max time duration alive (seconds)
        double m_time_to_live{ 2.5 };
        // max travel distance (pixels) - uint32_t?
        double m_max_travel_dist{ 1000.0 * 1000.0 };
    };
}
