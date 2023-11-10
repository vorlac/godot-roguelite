#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/variant.hpp>

#include "core/constants.hpp"
#include "entity/controller/player_controller.hpp"
#include "singletons/console.hpp"
#include "util/engine.hpp"
#include "util/input.hpp"
#include "util/io.hpp"

namespace rl
{
    void PlayerController::process_action_input(godot::Input* const input, double delta_time)
    {
        if (input->is_action_pressed("shoot"))
            this->emit_signal(event::character_shoot);
    }

    void PlayerController::process_movement_input(godot::Input* const input, double delta_time)
    {
        auto velocity{ input->get_vector(input::action::move_left, input::action::move_right,
                                         input::action::move_up, input::action::move_down) };
        this->emit_signal(event::character_move, velocity, delta_time);
    }

    // void PlayerController::_notification(int notification)
    // {
    //     switch (notification)
    //     {
    //         case NOTIFICATION_PREDELETE:
    //             [[fallthrough]];
    //         case NOTIFICATION_UNPARENTED:
    //         {
    //             console::get()->clear_context();
    //             console::get()->stop_logging();
    //             break;
    //         }
    //         default:
    //             break;
    //     }

    //     auto console{ console::get() };
    //     console->print("PlayerController: {}", notification);
    //     CharacterController::_notification(notification);
    // }

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

        this->emit_signal(event::character_rotate, m_rotation_angle, delta_time);
    }
}
