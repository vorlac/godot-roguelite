#pragma once

#include "util/console.hpp"

#include <godot_cpp/classes/panel.hpp>

class godot::RichTextLabel;

namespace rl
{
    class Level;

    class MainDialog : public godot::Panel
    {
        GDCLASS(MainDialog, godot::Panel)
    public:
        MainDialog();
        ~MainDialog();

        void _ready() override;
        void _notification(int notification);

        static void _bind_methods()
        {
        }

    protected:
        rl::utils::console m_console{};
        rl::Level* m_level{ nullptr };
        godot::RichTextLabel* m_console_label{ nullptr };
    };
}
