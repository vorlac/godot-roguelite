#pragma once

#include "util/utils.hpp"

#ifdef WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
  #include <corecrt_io.h>
#endif

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
#include <godot_cpp/variant/variant.hpp>

namespace rl
{
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

    class MainInterfaceDiag : public godot::Panel
    {
        GDCLASS(MainInterfaceDiag, godot::Panel)
    public:
        MainInterfaceDiag()
        {
        }

        ~MainInterfaceDiag()
        {
            m_is_active = false;
            if (m_console_write_thread != nullptr)
                m_console_write_thread->join();
        }

        void _ready()
        {
            if (m_console == nullptr)
            {
                auto scene_root{ scene::tree::root_node(this) };
                m_console = rl::convert<godot::RichTextLabel>(
                    scene_root->find_child("ConsolePanel", true, false));
                debug::assert(m_console != nullptr);
            }
            //    static auto print_handler = [&](void* this_instance, const godot::String& text,
            //                                    bool is_error_txt, bool is_rich_txt) {
            //        m_console->add_text(std::move(text));
            //    };

            //    std::tuple<decltype(&print_handler), void*, ErrorHandlerFunc*> ph = std::tuple(
            //        &print_handler, reinterpret_cast<void*>(this), nullptr);
            //}
            /////////////////////////////////////////////////////////////////
            std::streambuf* stdout_stream{ std::cout.rdbuf() };
            std::cout.rdbuf(stdout_strstm.rdbuf());
            // std::cout << "Hello, World!" << std::endl;
            // std::cout.rdbuf(stdout_stream);
            // std::cout << stdout_strstm.str();
            ////////////////////////////////////////////////////////////////
            // static constexpr DWORD std_input_handle{ ((DWORD)-11) };
            // freopen_s(&fp, "CONOUT$", "w", fp);  // Redirect stream.
            // setvbuf(fp, nullptr, _IONBF, 0);     // Disable stream buffering.
            // RedirectStreamRL("CONOUT$", "w", reinterpret_cast<FILE*>(fp), std_input_handle);
            ////////////////////////////////////////////////////////////////
        }

        void _notification(int notification)
        {
            // TODO: fixme
            // stdio gets piped to console in new UI, but internal logging APIs don't.
            // need to hook into the C io file/stream handles so it can all be intercepted
            std::cout << "__notification__ => " << std::to_string(notification) << std::endl;
            log::info("Notification: " + godot::itos(notification));
            // TODO: remove
            stdout_strstm.flush();
            std::string test = stdout_strstm.str();

            switch (notification)
            {
                case NOTIFICATION_DRAW:
                {
                    godot::String str{ test.c_str() };
                    if (!str.is_empty())
                        m_console->append_text(str);
                    break;
                }
                default:
                    break;
            }
        }

        static void _bind_methods()
        {
        }

    protected:
        std::mutex m_lock{};
        std::ostringstream stdout_strstm{};
        std::atomic<bool> m_is_active{ true };
        std::thread* m_console_write_thread{ nullptr };
        std::thread* m_console_msg_thread{ nullptr };
        std::vector<godot::String> m_console_buffer{};
        godot::RichTextLabel* m_console{ nullptr };
    };
}
