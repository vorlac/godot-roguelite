#pragma once

#include "util/debug.hpp"
#include "util/io.hpp"

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_action.hpp>
#include <godot_cpp/classes/input_map.hpp>

namespace rl::inline utils
{
    struct input
    {
        struct map
        {
            static inline godot::InputMap* get()
            {
                godot::InputMap* mappings{ godot::InputMap::get_singleton() };
                debug::assert(mappings != nullptr);
                return mappings;
            }
        };

        static inline godot::Input* get()
        {
            godot::Input* input{ godot::Input::get_singleton() };
            debug::assert(input != nullptr);
            return input;
        }

        static inline void hide_cursor()
        {
            godot::Input* const input{ input::get() };
            input->set_mouse_mode(godot::Input::MOUSE_MODE_HIDDEN);
        }

        static inline void show_cursor()
        {
            godot::Input* const input{ input::get() };
            input->set_mouse_mode(godot::Input::MOUSE_MODE_VISIBLE);
        }

        static inline bool cursor_visible()
        {
            godot::Input* const input{ input::get() };
            return input->get_mouse_mode() == godot::Input::MOUSE_MODE_VISIBLE;
        }

        static inline void load_project_inputs()
        {
            auto input_map{ godot::InputMap::get_singleton() };
            if (input_map != nullptr) [[likely]]
                return input_map->load_from_project_settings();
            rl::log::error("godot::Input singleton not intitialized");
        }

        static inline void use_accumulated_inputs(bool enable)
        {
            auto input{ godot::Input::get_singleton() };
            if (input != nullptr) [[likely]]
                return input->set_use_accumulated_input(enable);
            rl::log::error("godot::Input singleton not intitialized");
        }
    };
}
