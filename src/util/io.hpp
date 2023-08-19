#pragma once

#include <godot_cpp/classes/editor_node3d_gizmo_plugin.hpp>
#include <godot_cpp/classes/engine_debugger.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace rl::inline utils
{
    struct log
    {
        enum DetailLevel : uint_fast8_t
        {
            None,
            Error,
            Warning,
            Info,
            Debug,
            Trace
        };

        static constexpr const DetailLevel DETAIL_LEVEL{ log::DetailLevel::Debug };

        static constexpr inline bool level_active(const log::DetailLevel level)
        {
            return level <= log::DETAIL_LEVEL;
        }

        template <typename TString>
        static inline void error(TString msg)
        {
            if (log::level_active(log::DetailLevel::Error))
                ERR_PRINT_ED(std::move(msg));
        }

        template <typename TString>
        static inline void warning(TString msg)
        {
            if (log::level_active(log::DetailLevel::Warning))
                WARN_PRINT_ED(std::move(msg));
        }

        template <typename TString>
        static inline void info(TString msg)
        {
            if (log::level_active(log::DetailLevel::Info))
                godot::UtilityFunctions::print(std::move(msg));
        }

        template <typename TString>
        static inline void trace(TString msg)
        {
            if (log::level_active(log::DetailLevel::Trace))
                godot::UtilityFunctions::print(std::move(msg));
        }

        // namespace asdf
        //{
        //     void EditorNode::_print_handler(void *p_this, const String &p_string, bool p_error,
        //     bool p_rich) {
        //      callable_mp_static(&EditorNode::_print_handler_impl).bind(p_string, p_error,
        //      p_rich).call_deferred();
        //    }

        //    void EditorNode::_print_handler_impl(const String &p_string, bool p_error, bool
        //    p_rich) {
        //     if (p_error) {
        //      singleton->log->add_message(p_string, EditorLog::MSG_TYPE_ERROR);
        //     } else if (p_rich) {
        //      singleton->log->add_message(p_string, EditorLog::MSG_TYPE_STD_RICH);
        //     } else {
        //      singleton->log->add_message(p_string, EditorLog::MSG_TYPE_STD);
        //     }
        //    }

        //    print_handler.printfunc = _print_handler;
        // print_handler.userdata = this;
        // add_print_handler(&print_handler);
        //}
    };
}
