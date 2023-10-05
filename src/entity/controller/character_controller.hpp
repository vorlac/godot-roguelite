#pragma once

#include "core/concepts.hpp"
#include "core/constants.hpp"
#include "util/bind.hpp"
#include "util/io.hpp"

#include <type_traits>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace rl
{
    class CharacterController : public godot::Node2D
    {
        GDCLASS(CharacterController, godot::Node2D);

    public:
        enum class InputMode
        {
            MouseAndKeyboard,
            Controller,
            AI
        };

    public:
        CharacterController() = default;
        virtual ~CharacterController() = default;

        void _process(double delta_time) override;

        virtual void process_action_input(godot::Input* const input, double delta_time)
        {
        }

        virtual void process_movement_input(godot::Input* const input, double delta_time)
        {
        }

        virtual void process_rotation_input(godot::Input* const input, double delta_time)
        {
        }

    protected:
        template <typename T>
            requires std::derived_from<T, CharacterController>
        static void bind_members()
        {
            log::print("CharacterController::bind_members<{}>() called",
                       to<std::string_view>(T::get_class_static()));
            log::print("    - signal: {} added", event::character_move);
            log::print("    - signal: {} added", event::character_rotate);
            log::print("    - signal: {} added", event::character_shoot);
            log::print("    - signal: {} added", event::position_changed);

            using character_move_signal_t = signal_binding<T, event::character_move>;
            using add_character_move_signal_binding_t
                = character_move_signal_t::template add<godot::Vector2, double>;
            add_character_move_signal_binding_t();

            using character_rotate_signal_t = signal_binding<T, event::character_rotate>;
            using add_character_rotate_signal_binding_t
                = character_rotate_signal_t::template add<double, double>;
            add_character_rotate_signal_binding_t();

            using character_shoot_signal_t = signal_binding<T, event::character_shoot>;
            using add_character_shoot_signal_binding_t
                = character_shoot_signal_t::template add<godot::Object*>;
            add_character_shoot_signal_binding_t();

            using position_changed_signal_t = signal_binding<T, event::position_changed>;
            using add_position_changed_signal_binding_t
                = position_changed_signal_t::template add<godot::Object*, godot::Vector2>;
            add_position_changed_signal_binding_t();
        }

        static void _bind_methods()
        {
            CharacterController::bind_members<CharacterController>();
        }

    protected:
        // the active input mode for character controls
        InputMode m_input_mode{ InputMode::MouseAndKeyboard };
        // target rotation
        double m_rotation_angle{ 0.0 };
        // elapsed runtime (seconds)
        double m_elapsed_time{ 0.0 };
    };
}
