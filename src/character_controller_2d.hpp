#pragma once

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot
{
    class CharacterController2D : public Node2D
    {
        GDCLASS(CharacterController2D, Node2D)

    public:
        void _ready() override;
        void _process(double delta_time) override;
        void _physics_process(double delta_time) override;

        // void _process(double delta);
        // void _physics_process(double delta);
        // void _enter_tree();
        // void _exit_tree();
        // void _ready();
        // void _input(const Ref<InputEvent>& event);
        // void _shortcut_input(const Ref<InputEvent>& event);
        // void _unhandled_input(const Ref<InputEvent>& event);
        // void _unhandled_key_input(const Ref<InputEvent>& event);
        // PackedStringArray _get_configuration_warnings() const;

        static void _bind_methods();

        // clang-format off
        inline Vector2 get_speed() const { return m_target_speed; }
        inline Vector2 get_max_speed() const { return m_max_speed; }
        inline Vector2 get_acceleration() const { return m_acceleration; }
        inline real_t get_friction() const { return m_friction; }

        inline void set_speed(const Vector2& speed) { m_target_speed = speed; }
        inline void set_max_speed(const Vector2& max_speed) { m_max_speed = max_speed; }
        inline void set_acceleration(const Vector2& accel) { m_acceleration = accel; }
        inline void set_friction(const real_t friction) { m_friction = friction; }

        // clang-format on

    private:
        // Elapsed time in ms since last update
        double m_elapsed_time{ 0.0 };
        // Rate of decelleration when movement inputs aren't active (units/ms)
        real_t m_friction{ 0.1 };
        // Rate of accelleration (units/ms)
        Vector2 m_acceleration{ 0.1, 0.1 };
        // Target character walk speed (units/ms)
        Vector2 m_target_speed{ 500.0, 500.0 };
        // Maximum allowable character speed (units/ms)
        Vector2 m_min_speed{ 0.0, 0.0 };
        // Maximum allowable character speed (units/ms)
        Vector2 m_max_speed{ 1500.0f, 1500.0f };
        // The current movement distance/direction that
        // was just applied to character position (units)
        Vector2 m_velocity{ 0.0, 0.0 };
        // previous rotation input value
        Vector2 m_prev_rotation{ 0.0, 0.0 };
        Vector2 m_target_rotation{ 0.0, 0.0 };
    };
}
