#pragma once

#include "util/concepts.hpp"

#include <tuple>
#include <type_traits>

#include <godot_cpp/core/type_info.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace rl::inline utils
{
    template <VariantCompatible T>
    struct variant_traits
    {
        using raw_type = std::type_identity_t<T>;
        using type_info = std::type_identity_t<godot::GetTypeInfo<std::remove_cvref_t<T>>>;
        static constexpr inline godot::Variant::Type variant_type =
            static_cast<godot::Variant::Type>(type_info::VARIANT_TYPE);
    };
}
