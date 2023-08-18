#pragma once

#include "util/utils.hpp"

//
// #include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/canvas_layer.hpp>

// #include <godot_cpp/classes/h_box_container.hpp>
// #include <godot_cpp/classes/h_split_container.hpp>
// #include <godot_cpp/classes/label.hpp>
// #include <godot_cpp/classes/label_settings.hpp>
// #include <godot_cpp/classes/menu_bar.hpp>
// #include <godot_cpp/classes/menu_button.hpp>
// #include <godot_cpp/classes/rich_text_effect.hpp>
// #include <godot_cpp/classes/rich_text_label.hpp>
// #include <godot_cpp/classes/v_box_container.hpp>

namespace rl
{
    class DebugOverlay : public godot::CanvasLayer
    {
        GDCLASS(DebugOverlay, godot::CanvasLayer)
    public:
        DebugOverlay()
        {
        }

        ~DebugOverlay()
        {
        }

    protected:
        static void _bind_methods()
        {
        }

    private:
        /* godot::Label* m_label_1{ memnew(godot::Label) };
         godot::Label* m_label_2{ memnew(godot::Label) };
         godot::Label* m_label_3{ memnew(godot::Label) };

         godot::HBoxContainer* m_horizontal_layout{ memnew(godot::HBoxContainer) };
         godot::VBoxContainer* m_vertical_layout{ memnew(godot::VBoxContainer) };*/
    };
}
