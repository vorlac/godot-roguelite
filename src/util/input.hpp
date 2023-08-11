#pragma once

#include "util/io.hpp"

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_action.hpp>
#include <godot_cpp/classes/input_map.hpp>

namespace rl
{
    struct input
    {
        struct map
        {
            static inline godot::InputMap* const get()
            {
                return godot::InputMap::get_singleton();
            }
        };

        static inline godot::Input* const get()
        {
            return godot::Input::get_singleton();
        }

        const static inline void load_project_inputs()
        {
            auto input_map{ godot::InputMap::get_singleton() };
            if (input_map != nullptr) [[likely]]
                return input_map->load_from_project_settings();
            rl::log::error("godot::Input singleton not intitialized");
        }

        const static inline void use_accumulated_inputs(bool enable)
        {
            auto input{ godot::Input::get_singleton() };
            if (input != nullptr) [[likely]]
                return input->set_use_accumulated_input(enable);
            rl::log::error("godot::Input singleton not intitialized");
        }
    };
}
