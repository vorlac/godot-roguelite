#pragma once

#include <concepts>
#include <optional>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <gdextension_interface.h>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace rl::inline utils
{

#if defined(_MSC_VER)
    // MSVC doesn't have a clean way to ignore custom
    // attributes within a namespace like clang or gcc
  #define node_property
  #define signal_callback
#elif defined(__GNUG__) || defined(__clang__)
    // these macros are used to define custom attributes to label signal functions.
    // they don't do anything other than making it easier to spot these functions
    // when reading the code or searching for them in an IDE.
  #define node_property   rl::godot_node_property
  #define signal_callback rl::godot_signal_callback
#endif

// wrapper for simpler signal/slot connections
#define SignalConnection(signal_name, func_name, ...)                             \
    std::pair<godot::String, godot::Callable>                                     \
    {                                                                             \
        signal_name, godot::Callable(this, #func_name __VA_OPT__(, ) __VA_ARGS__) \
    }

    namespace signal
    {
        template <typename T>
            requires std::derived_from<godot::Node, std::type_identity_t<T>>
        using params_t = std::vector<T>;

        using callback_connection_t = std::pair<godot::String, godot::Callable>;

        template <typename TNode>
            requires std::derived_from<TNode, godot::Node>
        struct binding
        {
            using node_t = typename std::type_identity_t<TNode>;

            binding(godot::String&& name, std::vector<godot::PropertyInfo>&& param_info)
                : signal_name{ std::forward<godot::String>(name) }
                , signal_params{ std::forward<std::vector<godot::PropertyInfo>>(param_info) }
            {
            }

            inline void add_to_classdb()
            {
                godot::ClassDB::add_signal(TNode::get_class_static(),
                                           godot::MethodInfo(signal_name, signal_params));
            }

            const godot::String signal_name{};
            const std::vector<godot::PropertyInfo> signal_params{};
        };
    }

    template <typename TGetter, typename TSetter>
    struct PropertyBinding
    {
        PropertyBinding(std::tuple<godot::String, godot::String>&& func_names,
                        std::tuple<TGetter, TSetter>&& func_callables,
                        std::tuple<godot::String, godot::Variant::Type>&& prop_desc)
            : getter_name{ std::get<0>(func_names) }
            , setter_name{ std::get<1>(func_names) }
            , getter_func{ std::get<0>(func_callables) }
            , setter_func{ std::get<1>(func_callables) }
            , property_name{ std::get<0>(prop_desc) }
            , property_type{ std::get<1>(prop_desc) }
        {
        }

        godot::String getter_name;
        godot::String setter_name;
        TGetter getter_func{ nullptr };
        TSetter setter_func{ nullptr };
        godot::String property_name;
        godot::Variant::Type property_type{ godot::Variant::NIL };
    };
}
