#pragma once

#include "util/utils.hpp"

namespace rl::debug
{
    enum class DebugLevel : uint_fast8_t
    {
        None,
        Error,
        Warning,
        Info,
        Debug,
        Trace
    };

    static constexpr DebugLevel DEBUG_LEVEL{ DebugLevel::Debug };

    constexpr static inline bool level(const rl::DebugLevel level)
    {
        return rl::detail::debug_level <= level;
    }

    constexpr static inline void info(const String& msg)
    {
        if constexpr (rl::debug::level(DebugLevel::Info))
            gdutils::print(msg);
    }
}
