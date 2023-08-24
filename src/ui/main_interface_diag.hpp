#pragma once

#include "core/console_capture.hpp"
#include "util/utils.hpp"

#include <atomic>
#include <fmt/format.h>
#include <mutex>
#include <ostream>
#include <sstream>
#include <thread>

#include <godot_cpp/classes/panel.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string.hpp>

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
