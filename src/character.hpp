#pragma once

#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/math.hpp>

namespace godot
{
    class Input;
    class InputMap;
    class UtilityFunctions;

    class Character : public CharacterBody2D
    {
        GDCLASS(Character, CharacterBody2D);

    private:
        using utils = godot::UtilityFunctions;

        enum class InputMode
        {
            MouseAndKeyboard,
            Controller
        };

    public:
        void _ready() override;
        void _enter_tree() override;
        void _exit_tree() override;
        void _process(double delta_time) override;
        void _physics_process(double delta_time) override;
        void _input(const Ref<InputEvent>& event) override;
        void _unhandled_input(const Ref<InputEvent>& event) override;
        void _unhandled_key_input(const Ref<InputEvent>& event) override;
        void _shortcut_input(const Ref<InputEvent>& event) override;
        static void _bind_methods();

        [[nodiscard]] double get_movement_speed() const;
        [[nodiscard]] double get_movement_friction() const;
        [[nodiscard]] double get_rotation_speed() const;
        void set_movement_speed(const double move_speed);
        void set_movement_friction(const double move_friction);
        void set_rotation_speed(const double rotation_speed);

    protected:
        InputMode get_input_mode(Input* const input);
        void process_movement_input(Input* input, double delta_time);
        void process_rotation_input(Input* input, double delta_time);

    private:
        // active input mode for character controls
        InputMode m_input_mode{ InputMode::MouseAndKeyboard };
        // Rate of acceleration/deceleration (unit/s/s)
        double m_movement_friction{ 5.0 };
        // Rate of acceleration/deceleration (unit/s/s)
        double m_rotation_speed{ 5.0 };
        // target movement speed (units/s)
        double m_movement_speed{ 500.0 };
        // target facing angle (radians)
        double m_rotation_angle{ 0.0 };
        // elapsed runtime (seconds)
        double m_elapsed_time{ 0.0 };
        // TODO: break this out into an editor tool
        constexpr static inline bool RL_DEBUG{ true };
    };
}
