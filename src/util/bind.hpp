#pragma once

#include "util/assert.hpp"
#include "util/concepts.hpp"
#include "util/conversions.hpp"
#include "util/function_traits.hpp"
#include "util/variant.hpp"

#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/string_name.hpp>

namespace rl::inline utils
{
    template <auto Method>
        requires std::is_member_function_pointer_v<decltype(Method)>
    struct method : public function_traits<decltype(Method)>
    {
        using traits_t = function_traits<decltype(Method)>;

        static constexpr void bind(std::string_view&& func_name)
        {
            static constexpr std::size_t tup_size = std::tuple_size_v<typename traits_t::arg_types>;
            if constexpr (tup_size == 0)
                godot::ClassDB::bind_method(godot::D_METHOD(func_name.data()), Method);
            else
            {
                const typename traits_t::arg_types func_args{};
                std::vector<godot::String> vec_strs = rl::detail::to_arg_vec(func_args);
                std::tuple arg_types_str{ detail::arg_vec_to_tuple<tup_size>(vec_strs) };

                std::apply(
                    [&](auto&&... args) {
                        godot::ClassDB::bind_method(godot::D_METHOD(func_name.data()), Method,
                                                    args...);
                    },
                    arg_types_str);
            }
        }
    };

    template <auto Function>
    struct callback_func : public function_traits<decltype(Function)>
    {
        using traits_t = function_traits<decltype(Function)>;

        static void bind(std::string_view&& func_name)
        {
            auto class_name = godot::StringName("Main");
            static constexpr std::size_t tup_size = std::tuple_size_v<typename traits_t::arg_types>;
            if constexpr (tup_size == 0)
                godot::ClassDB::bind_static_method(class_name, godot::D_METHOD(func_name.data()),
                                                   Function);
            else
            {
                const typename traits_t::arg_types func_args{};
                std::vector<godot::String> vec_strs = rl::detail::to_arg_vec(func_args);
                std::tuple arg_types_str{ detail::arg_vec_to_tuple<tup_size>(vec_strs) };

                std::apply(
                    [&](auto&&... args) {
                        godot::ClassDB::bind_static_method(
                            class_name, godot::D_METHOD(func_name.data()), Function, args...);
                    },
                    func_args);
            }
        }
    };

    struct signal_registry
    {
        using signal_name_t = std::string;
        using signal_args_t = std::vector<godot::PropertyInfo>;
        using signal_properties_t = std::pair<signal_name_t, signal_args_t>;
        using signal_info_vec_t = std::vector<signal_properties_t>;
        using class_signal_map_t = std::unordered_map<std::string, signal_info_vec_t>;

        static inline class_signal_map_t class_bindings = {};
    };

    template <GDObjectDerived TObject, auto& SignalName>
    class signal_binding
    {
    public:
        using object_t = std::type_identity_t<TObject>;
        using signal_t = signal_binding<object_t, SignalName>;
        static inline constexpr std::string_view signal_name{ SignalName };
        static inline std::vector<godot::PropertyInfo> signal_params{};

        // even though we know what TObject is here (the class type adding the signal binding)
        // we can't call TObject::get_class_static() yet since this struct is instantiated before
        // the bindings library initializes the gdextension library, which will just lead to a
        // crash. this will just be set with the class name at runtime when signal_binding::add() is
        // invoked.
        static inline std::string class_name{};

    public:
        template <typename... TArgs>
        struct add
        {
            using arg_types = std::tuple<TArgs...>;
            static constexpr inline size_t arg_count{ std::tuple_size_v<arg_types> };

            add()
            {
                if (class_name.empty())
                    class_name = rl::to<std::string>(object_t::get_class_static());
                else
                {
                    std::string temp_name = rl::to<std::string>(object_t::get_class_static());
                    runtime_assert(class_name == temp_name);
                }

                if constexpr (arg_count == 0)
                    godot::ClassDB::add_signal(class_name.data(),
                                               godot::MethodInfo(signal_name.data()));
                else
                {
                    arg_types signal_args{};

                    std::apply(
                        [&](auto&&... arg) {
                            signal_params = {
                                variant_traits<decltype(arg)>::type_info::get_class_info()...
                            };
                        },
                        signal_args);

                    godot::ClassDB::add_signal(
                        class_name.data(),
                        godot::MethodInfo(signal_name.data(),
                                          std::forward<decltype(signal_params)>(signal_params)));
                }

                signal_registry::class_bindings[class_name].emplace_back(signal_name,
                                                                         signal_params);
                runtime_assert(signal_params.size() == arg_count);
            }
        };
    };

    template <auto&& SignalName>
    struct signal
    {
        static inline constexpr std::string_view signal_name{ SignalName };

        template <GDObjectDerived TOwnerObj>
        struct connect
        {
            static inline TOwnerObj* signal_owner{ nullptr };

            explicit connect(TOwnerObj* owner)
            {
                static_assert(std::is_same_v<decltype(owner), decltype(signal_owner)>);
                signal_owner = owner;

                bool found_binding{ false };
                const std::string class_name{
                    godot::String(TOwnerObj::get_class_static()).ascii().ptr()
                };
                const auto& class_signals = signal_registry::class_bindings[class_name];
                for (const auto& sig : class_signals)
                {
                    if (sig.first == signal_name)
                    {
                        found_binding = true;
                        break;
                    }
                }

                runtime_assert(found_binding);
            }

            auto operator<=>(godot::Callable&& callback)
            {
                // add_lambda_binding(func);

                // TODO: compare and validate the arg count, raw types, and variant conversions
                // between the matching signal binding record and the callback being connected.
                return signal_owner->connect(signal_name.data(), callback);
            }
        };
    };

    // TODO: get rid of this
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

#define bind_member_function(class_name, func_name) method<&class_name::func_name>::bind(#func_name)
#define slot(slot_owner, slot_callback)             godot::Callable(slot_owner, #slot_callback)

}
