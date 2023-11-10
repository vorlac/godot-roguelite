#pragma once

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include <fmt/compile.h>
#include <fmt/core.h>
#include <fmt/format-inl.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <godot_cpp/core/error_macros.hpp>

#include "core/concepts.hpp"

namespace rl::inline utils
{
    namespace io
    {
        static inline auto white(auto&& val)
        {
            return fmt::format(FMT_COMPILE("[color=white]{}[/color]"),
                               std::forward<decltype(val)>(val));
        }

        static inline auto gray(auto&& val)
        {
            return fmt::format(FMT_COMPILE("[color=gray]{}[/color]"),
                               std::forward<decltype(val)>(val));
        }

        static inline auto black(auto&& val)
        {
            return fmt::format(FMT_COMPILE("[color=black]{}[/color]"),
                               std::forward<decltype(val)>(val));
        }

        static inline auto red(auto&& val)
        {
            return fmt::format(FMT_COMPILE("[color=red]{}[/color]"),
                               std::forward<decltype(val)>(val));
        }

        static inline auto orange(auto&& val)
        {
            return fmt::format(FMT_COMPILE("[color=orange]{}[/color]"),
                               std::forward<decltype(val)>(val));
        }

        static inline auto yellow(auto&& val)
        {
            return fmt::format(FMT_COMPILE("[color=yellow]{}[/color]"),
                               std::forward<decltype(val)>(val));
        }

        static inline auto green(auto&& val)
        {
            return fmt::format(FMT_COMPILE("[color=green]{}[/color]"),
                               std::forward<decltype(val)>(val));
        }

        static inline auto blue(auto&& val)
        {
            return fmt::format(FMT_COMPILE("[color=blue]{}[/color]"),
                               std::forward<decltype(val)>(val));
        }

        static inline auto purple(auto&& val)
        {
            return fmt::format(FMT_COMPILE("[color=purple]{}[/color]"),
                               std::forward<decltype(val)>(val));
        }

    }

    struct log
    {
        enum DetailLevel : uint_fast8_t {
            None,
            Error,
            Warning,
            Info,
            Debug,
            Trace
        };

        static constexpr const DetailLevel DETAIL_LEVEL{ log::DetailLevel::Debug };

        static constexpr inline bool level_active(const log::DetailLevel level)
        {
            return level <= log::DETAIL_LEVEL;
        }

        template <typename TString>
        static inline void error(TString msg)
        {
            if (log::level_active(log::DetailLevel::Error))
                ERR_PRINT_ED(std::forward<TString>(msg));
        }

        template <typename TString>
        static inline void warning(TString msg)
        {
            if (log::level_active(log::DetailLevel::Warning))
                WARN_PRINT_ED(std::forward<TString>(msg));
        }

        template <typename TString>
        static inline void info(TString msg)
        {
            if (log::level_active(log::DetailLevel::Info))
                WARN_PRINT_ED(std::forward<TString>(msg));
        }

        template <typename TString>
        static inline void trace(TString msg)
        {
            if (log::level_active(log::DetailLevel::Trace))
                WARN_PRINT_ED(std::forward<TString>(msg));
        }

        template <typename... Args>
        static inline void print(spdlog::format_string_t<Args...> fmt, Args&&... args)
        {
            std::string msg{ fmt::format(fmt, std::forward<Args>(args)...) };
            ERR_PRINT_ED(msg.data());
        }
    };
}
