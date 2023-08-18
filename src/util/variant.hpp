#pragma once

#include <string_view>

#include <godot_cpp/variant/variant.hpp>

namespace rl::inline utils
{
    struct variant
    {
        [[nodiscard]]
        const static inline std::string_view to_string(godot::Variant&& var)
        {
            return std::string_view{ var.stringify().utf8().get_data() };
        }
    };
}
