#pragma once

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot
{
    using gdutils = godot::UtilityFunctions;

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

        namespace detail
        {
            static inline rl::DebugLevel debug_level{ rl::DebugLevel::Debug };
        }

        namespace util
        {
            constexpr static inline bool debug_active(const rl::DebugLevel level)
            {
                return rl::detail::debug_level <= level;
            }
        }

        namespace project
        {
            constexpr static inline auto project_path(const String& rel_path) -> String&&
            {
                return std::move("res://" + rel_path);
            }

            constexpr static inline auto user_data_path(const String& rel_path) -> String&&
            {
                return std::move("user://" + rel_path);
            }
        }

        namespace editor
        {
            const static inline bool active()
            {
                return godot::Engine::get_singleton()->is_editor_hint();
            }
        }

        namespace game
        {
            const static inline bool active()
            {
                return !editor::active();
            }
        }
    }
}
