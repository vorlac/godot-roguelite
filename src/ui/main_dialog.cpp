#include "ui/main_dialog.hpp"

#include "singletons/console.hpp"
#include "util/assert.hpp"
#include "util/conversions.hpp"

#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/control.hpp>

namespace rl::inline ui
{
    void MainDialog::_ready()
    {
        m_level = gdcast<Level>(this->find_child("Level1", true, false));
        runtime_assert(m_level != nullptr);

        godot::Node* scene_root{ scene::tree::root_node(this) };
        m_console_label = gdcast<godot::RichTextLabel>(
            scene_root->find_child("ConsolePanel", true, false));

        auto console{ Console<godot::RichTextLabel>::get() };
        console->set_context(m_console_label);
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

        auto console{ Console<godot::RichTextLabel>::get() };
        console->print("notification: {}\n", notification);
    }
}
