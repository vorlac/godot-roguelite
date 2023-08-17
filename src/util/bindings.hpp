#pragma once

#include <optional>
#include <tuple>
#include <type_traits>
#include <vector>

#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace rl
{
    // clang-format off
    
    // [[signal_event]] attribute to make callback signals easy to spot/find
    #define signal_event __declspec(godot_signal_event)
    // [[signal_callback]] attribute to make callback signals easy to spot/find
    #define signal_callback __declspec(godot_signal_callback)
    // wrapper for simpler signal/slot connections
    #define SignalConnection(signal, func_name, ...) std::pair<godot::String, godot::Callable> { signal, godot::Callable(this, #func_name __VA_OPT__(,) __VA_ARGS__) }

    // clang-format on

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

    struct SignalBinding
    {
        SignalBinding(godot::String&& name, std::vector<godot::PropertyInfo>&& param_info)
            : name{ std::move(name) }
            , params{ std::move(param_info) }
        {
        }

        godot::String name{};
        std::vector<godot::PropertyInfo> params{};
    };
}
