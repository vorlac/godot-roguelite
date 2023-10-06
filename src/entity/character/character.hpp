#pragma once

#include "core/attributes.hpp"
#include "core/constants.hpp"
#include "entity/camera.hpp"
#include "singletons/console.hpp"
#include "util/bind.hpp"
#include "util/io.hpp"

#include <concepts>

#include <godot_cpp/classes/character_body2d.hpp>

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
        Character(CharacterController* controller = nullptr);
        virtual ~Character() = default;

        void _ready() override;

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
        template <typename T>
            requires std::derived_from<T, Character>
        static void bind_members()
        {
            log::print("Character::bind_members<{}>() called",
                       to<std::string_view>(T::get_class_static()));
            log::print("    - method: {} binded", "on_character_movement");
            log::print("    - method: {} binded", "on_character_rotate");
            log::print("    - method: {} binded", "on_character_shoot");
            log::print("    - property: {} added", "movement_speed");
            log::print("    - property: {} added", "movement_friction");
            log::print("    - property: {} added", "rotation_speed");
            log::print("    - signal: {} added", event::character_shoot);
            log::print("    - signal: {} added", event::spawn_projectile);

            bind_member_function(T, on_character_movement);
            bind_member_function(T, on_character_rotate);
            bind_member_function(T, on_character_shoot);

            bind_property(T, movement_speed, double);
            bind_property(T, movement_friction, double);
            bind_property(T, rotation_speed, double);

            using pos_changed_signal_t = signal_binding<T, event::position_changed>;
            using add_pos_changed_signal_binding_t
                = pos_changed_signal_t::template add<godot::Object*, godot::Vector2>;
            add_pos_changed_signal_binding_t();

            using spawn_projectile_signal_t = signal_binding<T, event::spawn_projectile>;
            using add_spawn_projectile_signal_binding_t
                = spawn_projectile_signal_t::template add<godot::Object*, godot::Vector2>;
            add_spawn_projectile_signal_binding_t();
        }

        static void _bind_methods()
        {
            Character::bind_members<Character>();
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
