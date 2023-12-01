#pragma once

#include <type_traits>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/vector2.hpp>

#include "core/concepts.hpp"
#include "core/constants.hpp"
#include "util/bind.hpp"

namespace rl
{
    class CharacterController : public godot::Node2D
    {
        GDCLASS(CharacterController, godot::Node2D);

    public:
        enum class InputMode {
            MouseAndKeyboard,
            Controller,
            AI
        };

    public:
        CharacterController() = default;
        virtual ~CharacterController() = default;

        void _process(double delta_time) override;

        virtual void process_action_input(godot::Input* const input, double delta_time);
        virtual void process_movement_input(godot::Input* const input, double delta_time);
        virtual void process_rotation_input(godot::Input* const input, double delta_time);

    protected:
        static void _bind_methods();

    protected:
        // the active input mode for character controls
        InputMode m_input_mode{ InputMode::MouseAndKeyboard };
        // target rotation
        double m_rotation_angle{ 0.0 };
        // elapsed runtime (seconds)
        double m_elapsed_time{ 0.0 };
    };
}
