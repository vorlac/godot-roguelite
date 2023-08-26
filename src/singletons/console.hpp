#pragma once

#include <chrono>
#include <concepts>
#include <memory>
#include <spdlog/sinks/callback_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <type_traits>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/rich_text_effect.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/core/class_db.hpp>

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

                        std::string tmp{ msg.payload.begin(), msg.payload.end() };
                        m_gui_console->append_text(tmp.c_str());
                    }
                }) };

            stderr_sink->set_level(spdlog::level::err);
            stdout_sink->set_level(spdlog::level::info);
            callbk_sink->set_level(spdlog::level::debug);

            m_logger = std::unique_ptr<spdlog::logger>(
                new spdlog::logger{ "custom_callback_logger",
                                    { stdout_sink, stderr_sink, callbk_sink } });

            m_logger->info("some info log");
            m_logger->error("critical issue");  // will notify you

            using namespace std::chrono_literals;
            spdlog::flush_every(0.25s);
        }

        void info_msg(std::string&& msg)
        {
            m_logger->info(std::forward<std::string>(msg));
        }

        static inline rl::Console<TContext>* get()
        {
            return m_static_inst;
        }

    protected:
        static void _bind_methods()
        {
        }

    private:
        std::unique_ptr<spdlog::logger> m_logger{ nullptr };
        std::atomic<bool> m_stop{ false };
        TContext* m_gui_console{ nullptr };

    private:
        static inline rl::Console<TContext>* m_static_inst{ nullptr };
    };
}
