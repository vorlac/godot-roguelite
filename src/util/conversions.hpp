#pragma once

#include <concepts>
#include <string>
#include <string_view>
#include <type_traits>

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/variant/char_string.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/string_name.hpp>

namespace rl::inline utils
{
    template <typename TOut, typename TIn>
        requires std::derived_from<std::remove_cvref_t<TIn>, godot::Object>
    constexpr inline TOut* gdcast(TIn* obj)
    {
        return godot::Object::cast_to<TOut>(obj);
    }

    template <typename TOut>
    inline auto to(auto in) -> TOut
    {
        return TOut(in);
    }

    template <>
    inline auto to(godot::String in) -> std::string
    {
        return std::string(in.ascii().ptr());
    }

    template <>
    inline auto to(godot::String in) -> std::string_view
    {
        return std::string_view(in.ascii().ptr());
    }

    template <>
    inline auto to(godot::StringName in) -> std::string
    {
        return std::string(godot::String(in).ascii().ptr());
    }

    template <>
    inline auto to(godot::StringName in) -> std::string_view
    {
        return std::string_view(godot::String(in).ascii().ptr());
    }
}
