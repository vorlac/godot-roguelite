#pragma once

#include "ui/main_dialog.hpp"

#include "core/level.hpp"
#include "util/utils.hpp"

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
        debug::assert(m_level != nullptr);

        if (m_console_label == nullptr)
        {
            godot::Node* scene_root{ scene::tree::root_node(this) };
            m_console_label = rl::as<godot::RichTextLabel>(
                scene_root->find_child("ConsolePanel", true, false));

            debug::assert(m_console_label != nullptr);
            m_console.set_output_label(m_console_label);
            m_console.enable(true);
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
                m_console.enable(false);
                m_console.set_output_label(nullptr);
                break;
            }
            case Control::NOTIFICATION_MOUSE_ENTER:
            {
                debug::assert(m_level != nullptr);
                m_level->activate(true);
                break;
            }
            case Control::NOTIFICATION_MOUSE_EXIT:
            {
                debug::assert(m_level != nullptr);
                m_level->activate(false);
                break;
            }
        }

        m_console.print("notification: {}\n", notification::to_string(notification));
    }
}
