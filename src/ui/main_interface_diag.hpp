#pragma once

#include "util/console.hpp"

#include <godot_cpp/classes/panel.hpp>

class godot::RichTextLabel;

namespace rl
{
    class MainInterfaceDiag : public godot::Panel
    {
        GDCLASS(MainInterfaceDiag, godot::Panel)
    public:
        MainInterfaceDiag();
        ~MainInterfaceDiag();

        void _ready();
        void _notification(int notification);

        static void _bind_methods()
        {
        }

    protected:
        rl::utils::console m_console{};
        godot::RichTextLabel* m_console_label{ nullptr };
    };
}
