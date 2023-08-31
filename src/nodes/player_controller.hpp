#pragma once

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/node2d.hpp>

namespace rl
{
    class PlayerController : public godot::Node2D
    {
        GDCLASS(PlayerController, godot::Node2D);

    public:
        PlayerController();
        ~PlayerController();

        void _process(double delta_time) override;

        void process_state_input(godot::Input* const input, double delta_time);
        void process_movement_input(godot::Input* const input, double delta_time);
        void process_rotation_input(godot::Input* const input, double delta_time);

    protected:
        enum class InputMode
        {
            MouseAndKeyboard,
            Controller
        };

        InputMode get_input_mode(godot::Input* const input);

        static void _bind_methods();
        static void bind_signals();

    protected:
        // the active input mode for character controls
        InputMode m_input_mode{ InputMode::MouseAndKeyboard };
        // target rotation
        double m_rotation_angle{ 0.0 };
        // elapsed runtime (seconds)
        double m_elapsed_time{ 0.0 };
    };
}
