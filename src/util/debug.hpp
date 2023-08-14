#pragma once

#include <array>
#include <cstdio>
#include <map>
#include <tuple>
#include <type_traits>
#include <utility>

#include <godot_cpp/core/error_macros.hpp>

namespace rl::debug
{
    [[msvc::flatten]]
    static inline constexpr void assert(const bool cond)
    {
        // print condition and break debugger when cond is false.
        // calls to this function and the consitions are replaced with (void(0))
        // when building in release mode.
        DEV_ASSERT(cond);
    }
}

namespace rl::diag
{
    enum Option : uint_fast8_t
    {
        RootProcess,
        RootPhysics,
        RootViewport,
        RootInputs,
        LevelLoad,
        LevelUnload,
        MainMenuInit,
        All
    };

    using optval_t = std::pair<Option, bool>;
    static constexpr inline auto DebugSettings{
        std::to_array<optval_t>({
            { Option::All, false },
            { Option::RootProcess, true },
            { Option::RootPhysics, true },
            { Option::RootViewport, true },
            { Option::RootInputs, true },
            { Option::LevelLoad, true },
            { Option::LevelUnload, true },
            { Option::MainMenuInit, true },
        }),
    };

    static constexpr inline bool is_enabled(const Option diag_type)
    {
        for (const auto [opt, val] : DebugSettings)
        {
            if (opt == Option::All && val)
                return true;
            if (opt == diag_type)
                return val;
        }
        return false;
    }
}
