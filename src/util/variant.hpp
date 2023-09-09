#pragma once

#include <array>
#include <cstdint>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <utility>

#include <godot_cpp/core/type_info.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace rl::inline utils
{
    template <typename T>
    concept VariantConstructable = requires(const T& t) { godot::Variant(t); };

    template <typename T>
    concept VariantConvertable = requires(T t) { godot::Variant(t).operator T(); };

    template <typename T>
    concept VariantCompatible = VariantConstructable<std::remove_cvref_t<T>> &&
                                VariantConvertable<std::remove_cvref_t<T>> && requires {
                                    godot::GetTypeInfo<std::remove_cvref_t<T>>::VARIANT_TYPE;
                                };

    template <VariantCompatible T>
    struct variant_traits
    {
        using raw_type = std::type_identity_t<T>;
        using type_info = std::type_identity_t<godot::GetTypeInfo<std::remove_cvref_t<T>>>;
        static constexpr inline godot::Variant::Type variant_type =
            static_cast<godot::Variant::Type>(type_info::VARIANT_TYPE);
    };
}
