#pragma once

#include <godot_cpp/classes/engine_debugger.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace rl
{
    struct log
    {
        enum class DetailLevel : uint_fast8_t
        {
            None,
            Error,
            Warning,
            Info,
            Debug,
            Trace
        };

        static constexpr inline DetailLevel DETAIL_LEVEL = DetailLevel::Debug;

        [[msvc::flatten]]
        static constexpr inline bool level_active(const DetailLevel level)
        {
            return level <= log::DETAIL_LEVEL;
        }

        [[msvc::flatten]]
        static inline void error(const char* const msg)
        {
            if constexpr (log::level_active(DetailLevel::Error))
                ERR_PRINT_ED(msg);
        }

        [[msvc::flatten]]
        static inline void warning(const char* const msg)
        {
            if constexpr (log::level_active(DetailLevel::Warning))
                WARN_PRINT_ED(msg);
        }

        template <typename TString>
        [[msvc::flatten]] static inline void info(TString msg)
        {
            if (log::level_active(DetailLevel::Info))
                godot::UtilityFunctions::print(std::move(msg));
        }

        template <typename TString>
        [[msvc::flatten]] static inline void trace(TString msg)
        {
            // godot::UtilityFunctions::print_verbose(msg);
            if (log::level_active(DetailLevel::Trace))
                godot::UtilityFunctions::print_verbose(std::move(msg));
        }
    };
}
