#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/core/method_ptrcall.hpp>
#include <godot_cpp/core/type_info.hpp>

#include "util/input.hpp"

namespace rl::inline utils
{
    godot::InputMap* input::map::get()
    {
        godot::InputMap* mappings{ godot::InputMap::get_singleton() };
        return mappings;
    }

    godot::Input* input::get()
    {
        godot::Input* input{ godot::Input::get_singleton() };
        return input;
    }

    void input::hide_cursor()
    {
        godot::Input* const input{ input::get() };
        input->set_mouse_mode(godot::Input::MOUSE_MODE_HIDDEN);
    }

    void input::show_cursor()
    {
        godot::Input* const input{ input::get() };
        input->set_mouse_mode(godot::Input::MOUSE_MODE_VISIBLE);
    }

    bool input::cursor_visible()
    {
        godot::Input* const input{ input::get() };
        return input->get_mouse_mode() == godot::Input::MOUSE_MODE_VISIBLE;
    }

    void input::load_project_inputs()
    {
        auto input_map{ godot::InputMap::get_singleton() };
        return input_map->load_from_project_settings();
    }

    void input::use_accumulated_inputs(bool enable)
    {
        auto input{ godot::Input::get_singleton() };
        return input->set_use_accumulated_input(enable);
    }
}
