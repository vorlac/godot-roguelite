#pragma once

#include <godot_cpp/core/error_macros.hpp>

namespace rl::debug
{
    static inline constexpr void assert(const bool cond)
    {
        // print condition and break debugger when cond is false.
        // calls to this function and the consitions are replaced with (void(0))
        // when building in release mode.
        DEV_ASSERT(cond);
    }
}
