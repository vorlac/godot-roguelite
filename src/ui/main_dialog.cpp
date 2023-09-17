#include "ui/main_dialog.hpp"

#include "singletons/console.hpp"
#include "util/assert.hpp"
#include "util/conversions.hpp"

#include <fmt/format.h>

#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/control.hpp>

namespace rl::inline ui
{
    void MainDialog::_ready()
    {
        m_level = gdcast<rl::Level>(this->find_child("Level", true, false));
        runtime_assert(m_level != nullptr);

        if (m_console_label == nullptr)
        {
            godot::Node* scene_root{ scene::tree::root_node(this) };
            m_console_label = gdcast<godot::RichTextLabel>(
                scene_root->find_child("ConsolePanel", true, false));
            runtime_assert(m_console_label != nullptr);
            auto console{ Console<godot::RichTextLabel>::get() };
            runtime_assert(console != nullptr);
            console->set_context(m_console_label);
        }
    }

    void MainDialog::_notification(int notification)
    {
        switch (notification)
        {
            case Object::NOTIFICATION_PREDELETE:
                [[fallthrough]];
            case Node::NOTIFICATION_UNPARENTED:
            {
                Console<godot::RichTextLabel>::get()->clear_context();
                Console<godot::RichTextLabel>::get()->stop_logging();
                break;
            }
            case Control::NOTIFICATION_MOUSE_ENTER:
            {
                runtime_assert(m_level != nullptr);
                m_level->activate(true);
                break;
            }
            case Control::NOTIFICATION_MOUSE_EXIT:
            {
                runtime_assert(m_level != nullptr);
                m_level->activate(false);
                break;
            }
        }

        Console<godot::RichTextLabel>::get()->info_msg(
            fmt::format("notification: {}\n", notification));
    }
}
