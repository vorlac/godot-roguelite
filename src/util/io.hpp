#pragma once

#include <algorithm>
#include <concepts>
#include <functional>
#include <locale>
#include <memory>
#include <ranges>
#include <span>
#include <spdlog/spdlog.h>
#include <string>
#include <string_view>
#include <type_traits>

namespace rl::inline utils
{
    namespace io
    {
        // inline constexpr auto to_lowercase(std::string_view str) noexcept
        //{
        //     std::span<char> sp{ str.begin(), str.end() };
        //     std::transform(sp.begin(), sp.end(), sp.begin(), [](char c) -> char {
        //         if (c >= 'A' && c <= 'Z')
        //             return c - 32;
        //         return c;
        //     });
        //     return str.data();
        // }

        // constexpr const char* test{ std::move(to_lowercase("ASDF")) };
    }

    struct log
    {
        enum DetailLevel : uint_fast8_t
        {
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
            spdlog::debug(fmt, std::forward<Args>(args)...);
        }
    };
}
