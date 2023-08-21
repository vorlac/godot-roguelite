#pragma once
#include "ui/main_interface_diag.hpp"

#include "core/console_capture.hpp"
#include "util/utils.hpp"

#include <atomic>
#include <ios>
#include <mutex>
#include <ostream>
#include <sstream>
#include <thread>
#include <tuple>
#include <utility>

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
    void MainInterfaceDiag::test(int notification)
    {
    }

    MainInterfaceDiag::MainInterfaceDiag()
    {
        stdoutRedirect.Start();
    }

    MainInterfaceDiag::~MainInterfaceDiag()
    {
        stdoutRedirect.Stop();
    }

    void MainInterfaceDiag::_ready()
    {
        if (m_console == nullptr)
        {
            auto scene_root{ scene::tree::root_node(this) };
            m_console = rl::convert<godot::RichTextLabel>(
                scene_root->find_child("ConsolePanel", true, false));
            debug::assert(m_console != nullptr);
        }
    }

    void MainInterfaceDiag::_notification(int notification)
    {
        if (m_console != nullptr)
        {
            fmt::print("asdf");
            std::cout << "__notification__ => " << std::to_string(notification) << std::endl;
            log::info("log::[info]: " + godot::itos(notification));
            log::error("log::[error] : " + godot::itos(notification));
            log::warning("log::[warn] Notification: " + godot::itos(notification));
            printf("[printf]: notification = %d", notification);

            char buffer[512] = { 0 };
            size_t read_bytes = stdoutRedirect.GetBuffer(buffer, 512);
            if (read_bytes > 0)
            {
                godot::String out{ buffer };
                m_console->add_text(out);
                m_console->newline();
            }
        }
    }
}
