#pragma once

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
    // clang-format off
    
    // [[node_property]] attribute to make node properties easy spot/find
    #define node_property __declspec(godot_node_property)
    // [[signal_callback]] attribute to make callback signals easy to spot/find
    #define signal_callback __declspec(godot_signal_callback)
    // wrapper for simpler signal/slot connections
    #define SignalConnection(signal, func_name, ...) std::pair<godot::String, godot::Callable> { signal, godot::Callable(this, #func_name __VA_OPT__(,) __VA_ARGS__) }

    // clang-format on

    namespace signal
    {

        template <class T>
            requires std::derived_from<T, godot::Node>
        using params_t = std::vector<T>;

        using callback_connection_t = std::pair<godot::String, godot::Callable>;

        struct name
        {
            static constexpr inline auto position_changed{ "position_changed" };
            static constexpr inline auto entered_area{ "entered_area" };
            static constexpr inline auto exited_area{ "exited_area" };
            static constexpr inline auto shoot_projectile{ "shoot_projectile" };
        };

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

        godot::String getter_name{};
        godot::String setter_name{};
        godot::String property_name{};
        TGetter getter_func{ nullptr };
        TSetter setter_func{ nullptr };
        godot::Variant::Type property_type{ godot::Variant::NIL };
    };
}
