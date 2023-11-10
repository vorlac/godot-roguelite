#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/control.hpp>

#include "core/assert.hpp"
#include "core/constants.hpp"
#include "singletons/console.hpp"
#include "ui/main_dialog.hpp"
#include "util/conversions.hpp"
#include "util/engine.hpp"

namespace rl::inline ui
{
    void MainDialog::_ready()
    {
        if (engine::editor_active())
            return;

        Console<godot::RichTextLabel>* game_console{ console::get() };

        godot::Node* root{ scene::tree::root_node(this) };
        godot::Node* label{ root->find_child(name::dialog::console, true, false) };
        godot::Node* level{ this->find_child(name::level::level1, true, false) };

        m_level = gdcast<Level>(level);
        m_console_label = gdcast<godot::RichTextLabel>(label);

        game_console->set_context(m_console_label);
    }

    void MainDialog::_notification(int notification)
    {
        switch (notification)
        {
            case Object::NOTIFICATION_PREDELETE:
                [[fallthrough]];
            case Node::NOTIFICATION_UNPARENTED:
            {
                console::get()->clear_context();
                console::get()->stop_logging();
                break;
            }
            case Control::NOTIFICATION_MOUSE_ENTER:
            {
                m_level->activate(true);
                break;
            }
            case Control::NOTIFICATION_MOUSE_EXIT:
            {
                m_level->activate(false);
                break;
            }
        }

        auto console{ console::get() };
        console->print("notification: {}", notification);
    }
}
