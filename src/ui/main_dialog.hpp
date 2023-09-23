#pragma once

#include "nodes/level.hpp"

#include <godot_cpp/classes/panel.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>

namespace rl::inline ui
{
    class MainDialog : public godot::Panel
    {
        GDCLASS(MainDialog, godot::Panel);

    public:
        MainDialog() = default;
        ~MainDialog() = default;

        void _ready() override;
        void _notification(int notification);

        static void _bind_methods()
        {
        }

    protected:
        Level* m_level{ nullptr };
        godot::RichTextLabel* m_console_label{ nullptr };
    };
}
