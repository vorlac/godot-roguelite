#pragma once

#include <concepts>
#include <string>
#include <string_view>
#include <type_traits>

namespace godot
{
    template <typename T, typename V>
    struct GetTypeInfo;
    class Variant;
    class Object;
}

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

    template <typename T>
    concept VariantConstructable = requires(const T& t) { static_cast<godot::Variant>(t); };

    template <typename T>
    concept VariantConvertable = requires(T t) { static_cast<T>(t); };

    template <typename T>
    concept VariantCompatible =
        VariantConstructable<std::remove_cvref_t<T>> &&
        VariantConvertable<std::remove_cvref_t<T>> && requires {
            godot::GetTypeInfo<std::type_identity_t<std::remove_cvref_t<T>>, void>::VARIANT_TYPE;
        };
}
