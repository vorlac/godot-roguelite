#pragma once

#include "ui/main_interface_diag.hpp"

#include "util/utils.hpp"

#include <fmt/core.h>
#include <fmt/format.h>

#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>

namespace rl
{
    MainInterfaceDiag::MainInterfaceDiag()
    {
    }

    MainInterfaceDiag::~MainInterfaceDiag()
    {
    }

    void MainInterfaceDiag::_ready()
    {
        if (m_console_label == nullptr)
        {
            auto scene_root{ scene::tree::root_node(this) };
            m_console_label = rl::as<godot::RichTextLabel>(
                scene_root->find_child("ConsolePanel", true, false));

            debug::assert(m_console_label != nullptr);
            m_console.set_output_label(m_console_label);
            m_console.enable(true);
        }
    }

    void MainInterfaceDiag::_notification(int notification)
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
                input::hide_cursor(true);
                break;
            }
            case Control::NOTIFICATION_MOUSE_EXIT:
            {
                input::hide_cursor(false);
                break;
            }
        }

        m_console.print("notification: {}\n",
                        rl::to_string<Translation::Notification>(notification));
    }
}
