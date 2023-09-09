#pragma once

#include <concepts>
#include <string>
#include <string_view>
#include <type_traits>

#include <godot_cpp/core/object.hpp>

namespace rl::inline utils
{
    template <auto S>
    concept CompileTimeStr = std::same_as<decltype(S), std::string_view> ||
                             std::same_as<decltype(S), std::string> ||
                             std::same_as<decltype(S), const char*>;

    template <typename T>
    concept GDObjectDerived =
        std::derived_from<std::remove_cvref_t<std::remove_pointer_t<T>>, godot::Object> ||
        std::same_as<std::remove_cvref_t<std::remove_pointer_t<T>>, godot::Object>;
}
