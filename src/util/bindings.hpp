#pragma once

#include <optional>
#include <tuple>
#include <type_traits>

#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace rl
{
    using godot::PropertyInfo;
    using godot::String;
    using godot::Variant;

    template <typename TGetter, typename TSetter>
    struct PropertyBinding
    {
        PropertyBinding(std::tuple<String, String>&& func_names,
                        std::tuple<TGetter, TSetter>&& func_callables,
                        std::tuple<String, Variant::Type>&& prop_desc)
            : getter_name{ std::get<0>(func_names) }
            , setter_name{ std::get<1>(func_names) }
            , getter_func{ std::get<0>(func_callables) }
            , setter_func{ std::get<1>(func_callables) }
            , property_name{ std::get<0>(prop_desc) }
            , property_type{ std::get<1>(prop_desc) }
        {
        }

        String getter_name{};
        String setter_name{};
        String property_name{};
        TGetter getter_func{ nullptr };
        TSetter setter_func{ nullptr };
        Variant::Type property_type{ Variant::NIL };
    };

    struct SignalBinding
    {
        SignalBinding(String&& name, PropertyInfo&& recv_info, PropertyInfo&& send_info)
            : name{ std::move(name) }
            , sender_info{ std::move(send_info) }
            , receiver_info{ std::move(recv_info) }
        {
        }

        String name{};
        PropertyInfo sender_info{};
        PropertyInfo receiver_info{};
    };
}
