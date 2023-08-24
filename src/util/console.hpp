#pragma once

#include <atomic>
#include <chrono>
#include <fmt/compile.h>
#include <fmt/core.h>
#include <fmt/format-inl.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <iterator>
#include <mutex>
#include <string>
#include <string_view>
#include <type_traits>

#include <godot_cpp/classes/rich_text_label.hpp>

namespace rl::inline utils
{
    using namespace std::chrono_literals;

    class console
    {
    public:
        console(godot::RichTextLabel* output_label = nullptr)
            : m_output_console(output_label)
        {
            m_text_buffer.reserve(4096);
        }

        void set_output_label(godot::RichTextLabel* label)
        {
            m_output_console = label;
        }

        void enable(bool enable = true)
        {
            m_enabled.store(enable, std::memory_order_relaxed);
        }

        inline bool is_enabled() const
        {
            return m_enabled.load(std::memory_order_relaxed) && m_output_console != nullptr;
        }

        template <typename S, typename... Args>
        inline void print(const S& format_str, Args&&... args)
        {
            if (!this->is_enabled())
                return;

            fmt::format_to(std::back_inserter(m_text_buffer), fmt::runtime(format_str),
                           std::forward<Args>(args)...);
            std::string tmp{ m_text_buffer.data(), m_text_buffer.size() };
            m_output_console->append_text(tmp.c_str());
            m_text_buffer.clear();
        }

    private:
        using clock_t = std::chrono::high_resolution_clock;

        std::atomic<bool> m_enabled{ false };
        std::mutex m_mutex;

        clock_t::time_point m_last_update{ clock_t::now() };
        std::chrono::milliseconds m_udate_frequency = 100ms;
        fmt::basic_memory_buffer<char> m_text_buffer{};
        godot::RichTextLabel* m_output_console{ nullptr };
    };
}
