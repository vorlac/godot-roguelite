#pragma once
#include "ui/main_interface_diag.hpp"

#include "core/console_capture.hpp"
#include "util/utils.hpp"

#include <fmt/core.h>
#include <fmt/format.h>

#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/panel.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/core/builtin_ptrcall.hpp>
#include <godot_cpp/core/engine_ptrcall.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/core/method_ptrcall.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/variant.hpp>

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
            m_console_label = rl::convert<godot::RichTextLabel>(
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
            default:
            {
                m_console.print("notification: {}\n",
                                rl::to_string<Translation::Notification>(notification));
                break;
            }
        }
    }
}
