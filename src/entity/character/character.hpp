#pragma once

#include <concepts>

#include <godot_cpp/classes/character_body2d.hpp>

#include "core/attributes.hpp"
#include "core/constants.hpp"
#include "entity/camera.hpp"
#include "singletons/console.hpp"
#include "util/bind.hpp"

namespace godot
{
    class Marker2D;
    class Object;
    struct Vector2;
}

namespace rl
{
    class CharacterController;
}

namespace rl
{
    class Character : public godot::CharacterBody2D
    {
        GDCLASS(Character, godot::CharacterBody2D);

    public:
        Character();
        virtual ~Character() = default;

        virtual void _ready() override;

    public:
        CharacterController* get_controller() const;
        void set_controller(CharacterController* controller);

    protected:
        [[property]] double get_movement_speed() const;
        [[property]] double get_movement_friction() const;
        [[property]] double get_rotation_speed() const;

        [[property]] void set_movement_speed(const double move_speed);
        [[property]] void set_movement_friction(const double move_friction);
        [[property]] void set_rotation_speed(const double rotation_speed);

        [[signal_slot]] void on_character_shoot();
        [[signal_slot]] void on_character_rotate(double rotation_angle, double delta_time);
        [[signal_slot]] void on_character_movement(godot::Vector2 movement_velocity,
                                                   double delta_time);

    protected:
        static void _bind_methods()
        {
            bind_member_function(Character, on_character_movement);
            bind_member_function(Character, on_character_rotate);
            bind_member_function(Character, on_character_shoot);

            bind_property(Character, movement_speed, double);
            bind_property(Character, movement_friction, double);
            bind_property(Character, rotation_speed, double);

            signal_binding<Character, event::position_changed>::add<godot::Object*, godot::Vector2>();
            signal_binding<Character, event::spawn_projectile>::add<godot::Object*, godot::Vector2>();
        }

    protected:
        // Rate of acceleration/deceleration (unit/s/s)
        double m_movement_friction{ 5.0 };
        // Rate of rotational acceleration/deceleration (unit/s/s)
        double m_rotation_speed{ 10.0 };
        // target movement speed (units/s)
        double m_movement_speed{ 500.0 };
        // target facing angle (radians)
        double m_rotation_angle{ 0.0 };

        // the player character camera
        Camera* m_camera{ memnew(Camera) };
        // handles all input related player controls
        CharacterController* m_character_controller{ nullptr };
        // marker identifying location where to spwwn projectiles
        godot::Marker2D* m_firing_point{ nullptr };
    };
}
