#pragma once

#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace rl
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
}

namespace rl::log
{
    constexpr inline DebugLevel DEBUG_LEVEL{ DebugLevel::Debug };

    constexpr static inline bool level_active(const DebugLevel level)
    {
        return DEBUG_LEVEL <= level;
    }

    static inline void verbose(const char* const msg)
    {
        if constexpr (level_active(DebugLevel::Trace))
            godot::UtilityFunctions::print_verbose(msg);
    }

    static inline void info(const char* const msg)
    {
        if constexpr (level_active(DebugLevel::Info))
            godot::UtilityFunctions::print(msg);
    }

    static inline void warning(const char* const msg)
    {
        if constexpr (level_active(DebugLevel::Warning))
            WARN_PRINT_ED(msg);
    }

    static inline void error(const char* const msg)
    {
        if constexpr (level_active(DebugLevel::Error))
            ERR_PRINT_ED(msg);
    }

    static inline void assert(const bool cond)
    {
        // print condition and break debugger when cond is false.
        // calls to this function and the consitions are replaced with (void(0))
        // when building in release mode.
        DEV_ASSERT(cond);
    }
}
