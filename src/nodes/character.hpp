#pragma once

#include "nodes/camera.hpp"

#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/variant/string.hpp>

namespace rl
{
    using godot::CharacterBody2D;

    class Character : public godot::CharacterBody2D
    {
        GDCLASS(Character, godot::CharacterBody2D);

    public:
        enum class InputMode
        {
            MouseAndKeyboard,
            Controller
        };

        struct Signals
        {
            inline static constexpr auto PositionChanged{ "position_changed" };
        };

    public:
        Character();
        ~Character();

        void _ready() override;
        void _process(double delta_time) override;
        void _physics_process(double delta_time) override;
        void _input(const godot::Ref<godot::InputEvent>& event) override;

        static void _bind_methods();
        static void bind_signals();
        static void bind_properties();

    protected:
        [[nodiscard]] double get_movement_speed() const;
        [[nodiscard]] double get_movement_friction() const;
        [[nodiscard]] double get_rotation_speed() const;
        void set_movement_speed(const double move_speed);
        void set_movement_friction(const double move_friction);
        void set_rotation_speed(const double rotation_speed);

        InputMode get_input_mode(godot::Input* const input);
        void process_movement_input(godot::Input* input, double delta_time);
        void process_rotation_input(godot::Input* input, double delta_time);

    private:
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

        // the player character camera
        rl::Camera* m_camera{ memnew(rl::Camera) };
        // the player character's texture
        godot::Sprite2D* m_sprite{ memnew(godot::Sprite2D) };
        // the player character's collision area geometry
        godot::CollisionShape2D* m_collision_shape{ memnew(godot::CollisionShape2D) };
        // the active input mode for character controls
        InputMode m_input_mode{ InputMode::MouseAndKeyboard };
    };
}
