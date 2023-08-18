#pragma once

#include <godot_cpp/classes/os.hpp>

namespace rl::inline utils
{
    struct os
    {
        static inline godot::OS* const get()
        {
            return godot::OS::get_singleton();
        }
    };
}
