#include "entity/controller/character_controller.hpp"

#include "core/constants.hpp"
#include "util/bind.hpp"
#include "util/engine.hpp"
#include "util/input.hpp"
#include "util/io.hpp"

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace rl
{
    void CharacterController::_process(double delta_time)
    {
        if (engine::editor_active())
            return;

        godot::Input* input_handler{ input::get() };
        if (input_handler != nullptr)
        {
            this->process_movement_input(input_handler, delta_time);
            this->process_rotation_input(input_handler, delta_time);
            this->process_action_input(input_handler, delta_time);

            m_elapsed_time += delta_time;
            if (m_elapsed_time > 1.0)
            {
                m_elapsed_time = 0.0;
                this->emit_signal(event::position_changed, this->get_parent(),
                                  this->get_global_position());
            }
        }
    }

    // void CharacterController::process_action_input(godot::Input* const input, double delta_time)
    //{
    // }

    // void CharacterController::process_movement_input(godot::Input* const input, double delta_time)
    //{
    // }

    // void CharacterController::process_rotation_input(godot::Input* const input, double delta_time)
    //{
    // }

    // void CharacterController::_bind_methods()
    //{
    //     CharacterController::bind_members<CharacterController>();
    // }
}
