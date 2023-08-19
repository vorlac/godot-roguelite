#pragma once

#include <array>
#include <cstdio>
#include <map>
#include <tuple>
#include <type_traits>
#include <utility>

#include <godot_cpp/core/error_macros.hpp>

namespace rl::inline utils::debug
{
    static inline constexpr void assert(const bool cond)
    {
        DEV_ASSERT(cond);
    }
}

namespace rl::inline utils::diag
{
    enum Option : uint_fast8_t
    {
        RootProcess,
        RootPhysics,
        RootViewport,
        RootInputs,
        LevelProcess,
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
            { Option::LevelProcess, true },
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
