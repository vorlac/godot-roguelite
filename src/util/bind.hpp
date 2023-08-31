#pragma once

#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <gdextension_interface.h>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
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
    // these macros are used to define custom attributes to label godot signal callbacks
    // and node properties. they don't do anything other than making it easier to spot
    // these functions when reading the code or searching for them in an IDE.
  #define node_property   rl::godot_node_property
  #define signal_callback rl::godot_signal_callback
#endif

    /**
     * @brief macro to simplify and clarify the GDCLASS methods that are being binded.
     * class_name technically isn't needed since that's already deduced in detail::method_bind<>,
     * but it's more explicit this way and should help avoid mistakes by using this macro outside
     * of the method's class scope.
     *
     * @param class_name  The name of the method's class (not a string literal, must be unquoted)
     * @param func_name   The name of the member function being added to the bindings (not a string
     * literal, must be unquoted)
     */
#define add_method_binding(class_name, func_name) \
    signal::detail::method_bind<&class_name::func_name>::bind(#func_name)

    /**
     * @brief Container that holds onto all objects needed for signal connections
     * @brief using: @code{.cpp} std::tuple<godot::Object*, godot::String, godot::Callable>
     */
#define SignalConnection(signal_owner_obj, signal_name, callback_owner_obj, callback_func_name) \
    signal::connection                                                                          \
    {                                                                                           \
        signal_owner_obj, signal_name, godot::Callable                                          \
        {                                                                                       \
            callback_owner_obj, #callback_func_name                                             \
        }                                                                                       \
    }

#define BindSignals(signal_list)                                                            \
    do                                                                                      \
    {                                                                                       \
        for (auto&& sig : signal_list)                                                      \
        {                                                                                   \
            godot::ClassDB::add_signal(                                                     \
                get_class_static(),                                                         \
                godot::MethodInfo(sig.first, std::forward<signal::args_list>(sig.second))); \
        }                                                                                   \
    }                                                                                       \
    while (false)

    namespace signal
    {
        // the container type that holds all objects required for a signal connection
        using connection = std::tuple<godot::Object*, godot::String, godot::Callable>;
        // the container type that holds a signal's argument properties
        using args_list = std::vector<godot::PropertyInfo>;
        // the container type that holds all properties of a given signal
        using properties = std::pair<godot::String, args_list>;
        // the container type that holds all signal info needed to bind them for a single class
        using bindings = std::vector<signal::properties>;

        namespace detail
        {
            template <typename H>
            void to_arg_vec_impl(std::vector<godot::String>& s, H&& h)
            {
                s.push_back(typeid(decltype(h)).name());
            }

            template <typename H, typename... T>
            void to_arg_vec_impl(std::vector<godot::String>& s, H&& h, T&&... t)
            {
                s.push_back(typeid(decltype(h)).name());
                to_arg_vec_impl(s, std::forward<T>(t)...);
            }

            template <typename... TupleTypes, std::size_t... Idx>
            std::vector<godot::String> to_arg_vec(const std::tuple<TupleTypes...>& tup,
                                                  std::integer_sequence<std::size_t, Idx...>)
            {
                std::vector<godot::String> result = {};
                to_arg_vec_impl(result, std::get<Idx>(tup)...);
                return result;
            }

            template <typename... TupleTypes>
            std::vector<godot::String> to_arg_vec(const std::tuple<TupleTypes...>& tup)
            {
                static constexpr size_t arg_count = sizeof...(TupleTypes);
                return to_arg_vec(tup, std::make_index_sequence<arg_count>());
            }

            template <std::size_t... Indexes>
            auto arg_vec_to_tuple(const std::vector<godot::String>& v,
                                  std::index_sequence<Indexes...>)
            {
                return std::make_tuple(v[Indexes]...);
            }

            template <std::size_t N>
            auto arg_vec_to_tuple(const std::vector<godot::String>& v)
            {
                return arg_vec_to_tuple(v, std::make_index_sequence<N>());
            }

            template <typename = void>
            struct method_traits;

            template <typename Return, typename Object, typename... Args>
            struct method_traits<Return (Object::*)(Args...)>
            {
                using return_type = Return;
                using class_type = Object;
                using args_tuple = std::tuple<Args...>;
                inline static const std::string_view class_name{ typeid(class_type).name() };
                static constexpr size_t arg_count = sizeof...(Args);
            };

            template <typename Return, typename Object, typename... Args>
            struct method_traits<Return (Object::*)(Args...) const>
                : method_traits<Return (Object::*)(Args...)>
            {
                using class_type = const Object;
            };

            template <auto Method>
            struct method_bind : public method_traits<decltype(Method)>
            {
                using traits_t = method_traits<decltype(Method)>;

                static void bind(std::string_view&& func_name)
                {
                    static constexpr std::size_t tup_size = std::tuple_size_v<traits_t::args_tuple>;
                    if constexpr (tup_size > 0)
                    {
                        const traits_t::args_tuple func_args{};
                        std::vector<godot::String> vec_strs = to_arg_vec(func_args);
                        std::tuple arg_types_str{ std::move(arg_vec_to_tuple<tup_size>(vec_strs)) };

                        std::apply(
                            [&](auto&&... args) {
                                godot::ClassDB::bind_method(godot::D_METHOD(func_name.data()),
                                                            Method, args...);
                            },
                            arg_types_str);
                    }
                    else
                    {
                        godot::ClassDB::bind_method(godot::D_METHOD(func_name.data()), Method);
                    }
                }
            };
        }

        namespace todo
        {
            // TODO: use for propery bindings and remove the PropertyBinding struct below
            template <typename TNode, typename TTuple>
            int add_bindings(TTuple&& signal_bindings)
            {
                std::apply(
                    [&](auto&&... signal_info_elem) {
                        static auto func = [&](auto& signal_info...) {
                            using godot::ClassDB;
                            using godot::MethodInfo;

                            static inline auto class_name{
                                std::type_identity<TNode>::type::get_class_static()
                            };

                            auto signal_name = std::get<0>(signal_info);
                            auto signal_params = std::get<1>(signal_info);
                            ClassDB::add_signal(class_name, MethodInfo(signal_name, signal_params));
                        };

                        (func(signal_info_elem), ...);
                    },
                    signal_bindings);
            }
        }
    }

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
}
