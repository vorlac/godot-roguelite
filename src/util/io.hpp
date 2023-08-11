#pragma once

#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace rl::log
{

    enum class DetailLevel : uint_fast8_t
    {
        None,
        Error,
        Warning,
        Info,
        Debug,
        Trace
    };

    static inline constexpr DetailLevel DETAIL_LEVEL = DetailLevel::Debug;

    static inline constexpr bool level_active(const DetailLevel level)
    {
        return DETAIL_LEVEL <= level;
    }
}

namespace rl::log
{

    static inline constexpr void error(const char* const msg)
    {
        if constexpr (level_active(DetailLevel::Error))
            ERR_PRINT_ED(msg);
    }

    static inline constexpr void warning(const char* const msg)
    {
        if constexpr (level_active(DetailLevel::Warning))
            WARN_PRINT_ED(msg);
    }

    [[msvc::flatten]]
    static inline void info(const char* const msg)
    {
        if constexpr (level_active(DetailLevel::Info))
            godot::UtilityFunctions::print(msg);
    }

    [[msvc::flatten]]
    static inline void trace(const char* const msg)
    {
        // godot::UtilityFunctions::print_verbose(msg);
        if constexpr (level_active(DetailLevel::Trace))
            godot::UtilityFunctions::print(msg);
    }
}
