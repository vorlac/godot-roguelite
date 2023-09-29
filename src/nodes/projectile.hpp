#pragma once

#include "singletons/console.hpp"
#include "util/attributes.hpp"
#include "util/bind.hpp"
#include "util/conversions.hpp"
#include "util/io.hpp"

#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace rl
{
    class Projectile : public godot::Area2D
    {
        GDCLASS(Projectile, godot::Area2D);

    public:
        Projectile() = default;
        virtual ~Projectile() = default;

        void _ready() override;
        void _process(double delta_time) override;

        [[property]] double get_movement_speed() const;
        [[property]] double get_time_to_live() const;
        [[property]] double get_max_travel_dist() const;
        [[property]] double get_acceleration() const;
        [[property]] godot::Vector2 get_velocity() const;
        [[property]] void set_movement_speed(const double speed);
        [[property]] void set_time_to_live(const double ttl);
        [[property]] void set_max_travel_dist(const double dist);
        [[property]] void set_acceleration(const double acceleration);
        [[property]] void set_velocity(const godot::Vector2& velocity);

        // [[signal_slot]]
        // void on_body_enter(godot::Node* node) const
        // {
        //     console::get()->print("Projectile: {} {} {}", to<std::string>(this->get_name()),
        //                           io::yellow("entering >>"), to<std::string>(node->get_name()));
        // }

        // [[signal_slot]]
        // void on_body_exit(godot::Node* node) const
        // {
        //     console::get()->print("Projectile: {} {} {}", to<std::string>(this->get_name()),
        //                           io::orange("exiting <<"), to<std::string>(node->get_name()));
        // }

    protected:
        static void _bind_methods()
        {
            // bind_member_function(Projectile, on_body_enter);
            // bind_member_function(Projectile, on_body_exit);
        }

    protected:
        godot::Vector2 m_start_pos{ 0.0, 0.0 };
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
    };
}
