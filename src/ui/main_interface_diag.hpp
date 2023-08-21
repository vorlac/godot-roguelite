#pragma once

#include "core/console_capture.hpp"
#include "util/utils.hpp"

#ifdef WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
  #include <corecrt_io.h>
#endif

#include <atomic>
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

        //// std::wcout, std::wclog, std::wcerr, std::wcin
        // HANDLE hConOut = CreateFile(_T("CONOUT$"), GENERIC_READ | GENERIC_WRITE,
        //                             FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
        //                             FILE_ATTRIBUTE_NORMAL, NULL);
        // HANDLE hConIn = CreateFile(_T("CONIN$"), GENERIC_READ | GENERIC_WRITE,
        //                            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
        //                            FILE_ATTRIBUTE_NORMAL, NULL);
        // SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
        // SetStdHandle(STD_ERROR_HANDLE, hConOut);
        // SetStdHandle(STD_INPUT_HANDLE, hConIn);
        // std::wcout.clear();
        // std::wclog.clear();
        // std::wcerr.clear();
        // std::wcin.clear();
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
