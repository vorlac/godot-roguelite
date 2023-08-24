#pragma once

#include "util/bind.hpp"

#include <array>
#include <cstdio>
#include <map>
#include <tuple>
#include <type_traits>
#include <utility>

#include <godot_cpp/classes/engine_debugger.hpp>
#include <godot_cpp/core/error_macros.hpp>

namespace rl::inline utils::debug
{
    static inline constexpr void assert(const bool cond)
    {
        DEV_ASSERT(cond);
    }

    namespace engine::debugger
    {
        static inline godot::EngineDebugger* const get()
        {
            godot::EngineDebugger* debugger{ godot::EngineDebugger::get_singleton() };
            debug::assert(debugger != nullptr);
            return debugger;
        }

        static inline bool register_logger(signal::callback_connection_t&& callback)
        {
            godot::EngineDebugger* debugger{ engine::debugger::get() };
            debugger->register_message_capture(callback.first, callback.second);
            bool has_capture{ debugger->has_capture(callback.first) };
            debug::assert(has_capture);
            return has_capture;
        }

        static inline bool unregister_logger(godot::String&& name)
        {
            godot::EngineDebugger* debugger{ engine::debugger::get() };
            bool has_capture{ debugger->has_capture(name) };
            debug::assert(has_capture);
            debugger->unregister_message_capture(name);
            has_capture = debugger->has_capture(name);
            debug::assert(!has_capture);
            return !has_capture;
        }
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
