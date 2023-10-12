#pragma once

#include <concepts>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <vector>

#include <godot_cpp/variant/string.hpp>

#if defined(__GNUG__)
  #define stdcall __attribute__((stdcall))
#elif defined(__clang__)
  #define stdcall __stdcall
#elif defined(_MSC_VER)
  #define stdcall __cdecl
#endif

namespace rl::inline utils
{
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
        auto arg_vec_to_tuple(const std::vector<godot::String>& v, std::index_sequence<Indexes...>)
        {
            return std::make_tuple(v[Indexes]...);
        }

        template <std::size_t N>
        auto arg_vec_to_tuple(const std::vector<godot::String>& v)
        {
            return arg_vec_to_tuple(v, std::make_index_sequence<N>());
        }
    }

    template <typename>
    struct function_traits;

    // non-const member function
    template <typename TRet, typename TClass, typename... TArgs>
    struct function_traits<TRet (TClass::*)(TArgs...)>
    {
        using return_type = TRet;
        using class_type = TClass;
        using arg_types = std::tuple<TArgs...>;
        using arg_types_nocvref = std::tuple<std::remove_cvref_t<TArgs>...>;
        inline static const std::string_view class_name{ typeid(class_type).name() };
        static constexpr size_t arg_count = sizeof...(TArgs);
    };

    // const member function
    template <typename TRet, typename TClass, typename... TArgs>
    struct function_traits<TRet (TClass::*)(TArgs...) const>
        : public function_traits<TRet (TClass::*)(TArgs...)>
    {
        using class_type = const TClass;
    };

    // functor / lambda
    template <typename TCallable>
    struct function_traits
        : public function_traits<decltype(&std::remove_reference_t<TCallable>::operator())>
    {
        inline static const std::string_view class_name{};
    };

    // leaves function signature untouched if
    // __stdcall or __cdecl isn't specified
    template <typename TFunction>
    struct remove_callingconv
    {
        using type = TFunction;
    };

    // strips __stdcall or __cdecl out of
    // lambdas function/lambda type signatures
    template <typename TRet, typename... TArgs>
    struct remove_callingconv<TRet(stdcall*)(TArgs...)>
    {
        using type = TRet(TArgs...);
    };

    // strips __stdcall or __cdecl out of
    // noexcept function/lambda type signatures
    template <typename TRet, typename... TArgs>
    struct remove_callingconv<TRet stdcall(TArgs...) noexcept>
    {
        using type = TRet(TArgs...) noexcept;
    };
}
