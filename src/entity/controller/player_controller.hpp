#pragma once

#include "entity/controller/character_controller.hpp"

namespace rl
{
    class PlayerController : public CharacterController
    {
        GDCLASS(PlayerController, CharacterController);

    public:
        PlayerController() = default;
        ~PlayerController() = default;

        void process_action_input(godot::Input* const input, double delta_time) override;
        void process_movement_input(godot::Input* const input, double delta_time) override;
        void process_rotation_input(godot::Input* const input, double delta_time) override;
        InputMode get_input_mode(godot::Input* const input);

    protected:
        static void _bind_methods()
        {
        }
    };
}
