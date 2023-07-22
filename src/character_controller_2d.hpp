#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/sprite2d.hpp>

// clang-format off
namespace godot
{
    class CharacterController2D : public Sprite2D
    {
        GDCLASS(CharacterController2D, Sprite2D)

    public:
        CharacterController2D() { }
        ~CharacterController2D() { }

        void _ready() override;
        void _process(double delta_time) override;
        void _physics_process(double delta_time) override;
        static void _bind_methods();

        // virtual void _process(double delta);
        // virtual void _physics_process(double delta);
        // virtual void _enter_tree();
        // virtual void _exit_tree();
        // virtual void _ready();
        // virtual void _input(const Ref<InputEvent>& event);
        // virtual void _shortcut_input(const Ref<InputEvent>& event);
        // virtual void _unhandled_input(const Ref<InputEvent>& event);
        // virtual void _unhandled_key_input(const Ref<InputEvent>& event);
        // virtual PackedStringArray _get_configuration_warnings() const;

    protected:
        void process_movement(double delta_time);

        // movement property getters
        inline Vector2 get_speed() const { return m_target_speed; }
        inline Vector2 get_max_speed() const { return m_max_speed; }
        inline Vector2 get_acceleration() const { return m_acceleration; }
        inline Vector2 get_friction() const { return m_friction; }

        // movement property setters
        inline void set_speed(const Vector2& speed) { m_target_speed = speed; }
        inline void set_max_speed(const Vector2& max_speed) { m_max_speed = max_speed; }
        inline void set_acceleration(const Vector2& accel) { m_acceleration = accel; }
        inline void set_friction(const Vector2& friction) { m_friction = friction; }

    private:
        // Elapsed time in ms since last update
        double m_elapsed_time{ 0 };
        // Rate of decelleration when movement inputs aren't active (units/ms)
        Vector2 m_friction{ 0.1, 0.1 };
        // Rate of accelleration (units/ms)
        Vector2 m_acceleration{ 0.1, 0.1 };
        // Target character walk speed (units/ms)
        Vector2 m_target_speed{ 500.0, 500.0 };
        // Maximum allowable character speed (units/ms)
        Vector2 m_min_speed{ 0.0, 0.0 };
        // Maximum allowable character speed (units/ms)
        Vector2 m_max_speed{ 1500.0, 1500.0 };
        // The current movement distance/direction that
        // was just applied to character position (units)
        Vector2 m_velocity{ 0, 0 };
    };

}

// clang-format on
