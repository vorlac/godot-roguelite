#pragma once

#include <chrono>
#include <string>
#include <type_traits>

#include <fmt/chrono.h>
#include <fmt/compile.h>
#include <fmt/core.h>
#include <spdlog/sinks/callback_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>

namespace rl
{
    template <typename TContext>
    class Console : public godot::Object
    {
        GDCLASS(Console, godot::Object);

    public:
        Console()
        {
            m_static_inst = this;
            init_loggers();
        }

        ~Console()
        {
            m_static_inst = nullptr;
        }

        static inline rl::Console<TContext>* get()
        {
            return m_static_inst;
        }

        void set_context(TContext* context)
        {
            m_gui_console = context;
        }

        void clear_context()
        {
            m_gui_console = nullptr;
        }

        void stop_logging()
        {
            m_logger->flush();
            m_stop = true;
        }

        void init_loggers()
            requires std::same_as<TContext, godot::RichTextLabel>
        {
            auto stdout_sink{ std::make_shared<spdlog::sinks::stdout_color_sink_mt>() };
            auto stderr_sink{ std::make_shared<spdlog::sinks::stderr_color_sink_mt>() };
            auto callbk_sink{ std::make_shared<spdlog::sinks::callback_sink_mt>(
                [this](const spdlog::details::log_msg& msg) {
                    if (!m_stop.load(std::memory_order_relaxed))
                    {
                        if (m_gui_console == nullptr)
                            return;

                        using duration_t = std::chrono::duration<double>;
                        const duration_t elapsed{ clock_t::now() - m_start_time };

                        m_gui_console->append_text(
                            fmt::format("[color=gray]{:5} [{:>7.2}] [b]=>[/b] {}[/color]\n",
                                        m_line_num.fetch_add(1, std::memory_order_relaxed), elapsed,
                                        msg.payload)
                                .c_str());
                    }
                }) };

            stderr_sink->set_level(spdlog::level::err);
            stdout_sink->set_level(spdlog::level::info);
            callbk_sink->set_level(spdlog::level::debug);

            m_logger = std::unique_ptr<spdlog::logger>(
                new spdlog::logger{ "custom_callback_logger",
                                    { stdout_sink, stderr_sink, callbk_sink } });

            using namespace std::chrono_literals;
            spdlog::flush_every(0.25s);
        }

        template <typename... TArgs>
        void print(fmt::format_string<TArgs...> format_str, TArgs&&... args)
        {
            m_logger->info(format_str, std::forward<TArgs>(args)...);
        }

    protected:
        static void _bind_methods()
        {
        }

    private:
        std::unique_ptr<spdlog::logger> m_logger{ nullptr };
        std::atomic<bool> m_stop{ false };
        std::atomic<uint32_t> m_line_num{ 0 };
        TContext* m_gui_console{ nullptr };

        using clock_t = std::chrono::high_resolution_clock;
        const clock_t::time_point m_start_time{ clock_t::now() };

    private:
        static inline Console<TContext>* m_static_inst{ nullptr };
    };

    using console = Console<godot::RichTextLabel>;
}
