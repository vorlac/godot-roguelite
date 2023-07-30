#pragma once

#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot::rl
{
    using utils = godot::UtilityFunctions;

    static inline auto project_path(const String& rel_path) -> String&&
    {
        return std::move("res://" + rel_path);
    }

    static inline auto user_data_path(const String& rel_path) -> String&&
    {
        return std::move("user://" + rel_path);
    }
}
