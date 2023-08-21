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
    static constexpr inline auto profiler_cap = [](void* this_ptr, const godot::String& cmd,
                                                   const godot::Array& data, bool& captured) {
        return reinterpret_cast<MainInterfaceDiag*>(this_ptr)->message_callback(cmd, data,
                                                                                captured);
    };

    void MainInterfaceDiag::test(int notification)
    {
    }

    // static void RedirectStreamRL(const char* p_file_name, const char* p_mode, FILE* p_cpp_stream,
    //                              const DWORD p_std_handle)
    // {
    //     const HANDLE h_existing = GetStdHandle(p_std_handle);
    //     if (h_existing != INVALID_HANDLE_VALUE)
    //     {  // Redirect only if attached console have a valid handle.
    //         const HANDLE h_cpp = reinterpret_cast<HANDLE>(_get_osfhandle(_fileno(p_cpp_stream)));
    //         if (h_cpp == INVALID_HANDLE_VALUE)
    //         {  // Redirect only if it's not already redirected to the pipe or file.
    //             FILE* fp = p_cpp_stream;
    //             freopen_s(&fp, p_file_name, p_mode, p_cpp_stream);  // Redirect stream.
    //             setvbuf(p_cpp_stream, nullptr, _IONBF, 0);          // Disable stream buffering.
    //         }
    //     }
    // }

    // enum ErrorHandlerType
    // {
    //     ERR_HANDLER_ERROR,
    //     ERR_HANDLER_WARNING,
    //     ERR_HANDLER_SCRIPT,
    //     ERR_HANDLER_SHADER,
    // };

    // extern void (*_print_func)(godot::String);
    // typedef void (*PrintHandlerFunc)(void*, const godot::String& p_string, bool p_error,
    //                                  bool p_rich);
    // typedef void (*ErrorHandlerFunc)(void*, const char*, const char*, int p_line, const char*,
    //                                  const char*, bool p_editor_notify,
    //                                  rl::ErrorHandlerType p_type);

    MainInterfaceDiag::MainInterfaceDiag()
    {
        stdoutRedirect.Start();
    }

    MainInterfaceDiag::~MainInterfaceDiag()
    {
        stdoutRedirect.Stop();
        m_is_active = false;
        if (m_console_write_thread != nullptr)
            m_console_write_thread->join();
    }

    void MainInterfaceDiag::message_callback(const godot::String& cmd, const godot::Array& data,
                                             bool& captured)
    {
        m_console->add_text("cmd=" + cmd + " data=" + godot::Variant(data).stringify() +
                            " cap=" + (captured ? "true" : "false") + "\n");
    }

    void MainInterfaceDiag::_ready()
    {
        static const auto asdf = [&](int notification) {
            while (m_is_active)
            {
                {
                    std::cout << "__notification__ => " << std::to_string(notification)
                              << std::endl;
                    log::info("log::[info]: " + godot::itos(notification));
                    log::error("log::[error] : " + godot::itos(notification));
                    log::warning("log::[warn] Notification: " + godot::itos(notification));
                    printf("[printf]: notification = %d", notification);

                    stdoutRedirect.Stop();
                    // std::scoped_lock<std::mutex> lock{ m_lock };
                    char buffer[512] = { 0 };
                    size_t read_bytes = stdoutRedirect.GetBuffer(buffer, 512);
                    if (read_bytes > 0)
                    {
                        std::string_view out{ buffer, read_bytes };
                        m_console->add_text(out.data());
                    }
                }
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(0.1s);
            }
        };

        // m_is_active = true;
        //  m_console_write_thread = new std::thread(asdf, 0);

        if (m_console == nullptr)
        {
            auto scene_root{ scene::tree::root_node(this) };
            m_console = rl::convert<godot::RichTextLabel>(
                scene_root->find_child("ConsolePanel", true, false));
            debug::assert(m_console != nullptr);
        }
        //    std::streambuf* stdout_stream{ std::cout.rdbuf() };
        //    std::cout.rdbuf(stdout_strstm.rdbuf());

        //    debug::engine::debugger::register_logger(
        //        SignalConnection("rl_message_handler", &handle_messages));

        //    // CreateConsole();
        //}
    }

    void MainInterfaceDiag::_notification(int notification)
    {
        // TODO: fixme
        // stdio gets piped to console in new UI, but internal logging APIs don't.
        // need to hook into the C io file/stream handles so it can all be intercepted
        if (m_console != nullptr)
        {
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

        // TODO: remove
        // stdout_strstm.flush();
        // std::string test = stdout_strstm.str();
        // godot::String str{ test.c_str() };
        // if (!str.is_empty() && m_console != nullptr)
        //{
        //    // m_console->add_text(str);
        //    // m_console->newline();
        //}
        // switch (notification)
        //{
        //    case NOTIFICATION_DRAW:
        //    {
        //        if (!str.is_empty())
        //        {
        //            // m_console->append_text(str);
        //            // m_console->queue_redraw();
        //        }
        //        break;
        //    }
        //    default:
        //        break;
        //}
    }

}
