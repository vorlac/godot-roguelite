#pragma once

#include <array>
#include <cstdint>
#include <tuple>
#include <type_traits>
#include <utility>

#include <godot_cpp/core/error_macros.hpp>

namespace rl::inline utils
{
    namespace diag
    {
        enum Option : uint_fast8_t {
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
    }
}
