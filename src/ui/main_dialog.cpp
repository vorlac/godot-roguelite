#include "ui/main_dialog.hpp"

#include "core/level.hpp"
#include "singletons/console.hpp"
#include "util/debug.hpp"

#include <fmt/core.h>
#include <fmt/format.h>

#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>

namespace rl
{
    MainDialog::MainDialog()
    {
        this->set_name("MainDialog");
    }

    MainDialog::~MainDialog()
    {
    }

    void MainDialog::_ready()
    {
        m_level = rl::as<rl::Level>(this->find_child("Level", true, false));
        debug::runtime_assert(m_level != nullptr);

        if (m_console_label == nullptr)
        {
            godot::Node* scene_root{ scene::tree::root_node(this) };
            m_console_label = rl::as<godot::RichTextLabel>(
                scene_root->find_child("ConsolePanel", true, false));

            debug::runtime_assert(m_console_label != nullptr);
            auto console{ Console<godot::RichTextLabel>::get() };
            debug::runtime_assert(console != nullptr);
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
                debug::runtime_assert(m_level != nullptr);
                m_level->activate(true);
                break;
            }
            case Control::NOTIFICATION_MOUSE_EXIT:
            {
                debug::runtime_assert(m_level != nullptr);
                m_level->activate(false);
                break;
            }
        }

        Console<godot::RichTextLabel>::get()->info_msg(
            fmt::format("=> notification: {}\n", notification));
    }
}
