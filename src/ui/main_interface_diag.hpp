#pragma once

#include "core/console_capture.hpp"
#include "util/utils.hpp"

#ifdef WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
  #include <corecrt_io.h>
#endif

#include <atomic>
#include <fmt/format.h>
#include <mutex>
#include <ostream>
#include <sstream>
#include <thread>

#include <godot_cpp/classes/panel.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string.hpp>

namespace rl
{
    static void CreateConsole()
    {
        if (!AllocConsole())
        {
            // Add some error handling here.
            // You can call GetLastError() to get more info about the error.
            return;
        }

        // std::cout, std::clog, std::cerr, std::cin
        FILE* fDummy;
        freopen_s(&fDummy, "CONOUT$", "w", stdout);
        freopen_s(&fDummy, "CONOUT$", "w", stderr);
        freopen_s(&fDummy, "CONIN$", "r", stdin);
        std::cout.clear();
        std::clog.clear();
        std::cerr.clear();
        std::cin.clear();
    }

    class MainInterfaceDiag : public godot::Panel
    {
        GDCLASS(MainInterfaceDiag, godot::Panel)
    public:
        MainInterfaceDiag();

        ~MainInterfaceDiag();
        void test(int notification);

        void message_callback(const godot::String& cmd, const godot::Array& data, bool& captured);

        void _ready();

        void _notification(int notification);

        static void _bind_methods()
        {
        }

    protected:
        std::mutex m_lock{};
        std::ostringstream stdout_strstm{};
        std::atomic<bool> m_is_active{ false };
        std::thread* m_console_write_thread{ nullptr };
        std::thread* m_console_msg_thread{ nullptr };
        std::vector<godot::String> m_console_buffer{};
        godot::RichTextLabel* m_console{ nullptr };
        StdOutRedirect stdoutRedirect{ 512 };
    };
}
