#include "nodes/player_controller.hpp"

#include "util/bind.hpp"
#include "util/constants.hpp"
#include "util/engine.hpp"
#include "util/input.hpp"
#include "util/io.hpp"

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace rl::inline node
{
    void PlayerController::_process(double delta_time)
    {
        if (engine::editor_active())
            return;

        godot::Input* input_handler{ input::get() };
        if (input_handler != nullptr)
        {
            this->process_movement_input(input_handler, delta_time);
            this->process_rotation_input(input_handler, delta_time);
            this->process_state_input(input_handler, delta_time);

            m_elapsed_time += delta_time;
            if (m_elapsed_time > 1.0)
            {
                m_elapsed_time = 0.0;
                this->emit_signal(event::position_changed, this->get_parent(),
                                  this->get_global_position());
            }
        }
    }

    void PlayerController::process_state_input(godot::Input* const input, double delta_time)
    {
        if (input->is_action_pressed("shoot"))
            this->emit_signal(event::player_shoot);
    }

    void PlayerController::process_movement_input(godot::Input* const input, double delta_time)
    {
        auto velocity{ input->get_vector("move_left", "move_right", "move_up", "move_down") };
        this->emit_signal(event::player_move, velocity, delta_time);
    }

    PlayerController::InputMode PlayerController::get_input_mode(godot::Input* const input)
    {
        switch (m_input_mode)
        {
            default:
                [[fallthrough]];
            case InputMode::MouseAndKeyboard:
            {
                bool controller_input_detected{ input->is_action_pressed("controller_any") };
                if (controller_input_detected)
                    m_input_mode = InputMode::Controller;
                break;
            }
            case InputMode::Controller:
            {
                godot::Vector2 mouse_velocity{ input->get_last_mouse_velocity() };
                if (!mouse_velocity.is_zero_approx())
                    m_input_mode = InputMode::MouseAndKeyboard;
                break;
            }
        }

        return m_input_mode;
    }

    void PlayerController::process_rotation_input(godot::Input* const input, double delta_time)
    {
        switch (this->get_input_mode(input))
        {
            default:
                [[fallthrough]];
            case InputMode::MouseAndKeyboard:
            {
                godot::Vector2 rotation_dir{ this->get_global_mouse_position() -
                                             this->get_global_position() };

                m_rotation_angle = rotation_dir.angle() + godot::Math::deg_to_rad(90.0);
                break;
            }
            case InputMode::Controller:
            {
                godot::TypedArray<int32_t> controllers{ input->get_connected_joypads() };
                if (controllers.is_empty())
                    log::warning("InputMode = Controller, but no controllers detected");
                else
                {
                    godot::Vector2 target_rotation{ input->get_vector("rotate_left", "rotate_right",
                                                                      "rotate_up", "rotate_down") };
                    if (!target_rotation.is_zero_approx())
                        m_rotation_angle = godot::Vector2(0, 0).angle_to_point(target_rotation) +
                                           godot::Math::deg_to_rad(90.0);
                }
                break;
            }
        }

        this->emit_signal(event::player_rotate, m_rotation_angle, delta_time);
    }

    void PlayerController::_bind_methods()
    {
        signal_binding<PlayerController, event::player_move>::add<godot::Vector2, double>();
        signal_binding<PlayerController, event::player_rotate>::add<double, double>();
        signal_binding<PlayerController, event::player_shoot>::add<godot::Object*>();
        signal_binding<PlayerController, event::position_changed>::add<godot::Object*, godot::Vector2>();
    }
}
