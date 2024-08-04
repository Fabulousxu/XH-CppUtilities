#ifndef FUNCTION_TRAITS_H
#define FUNCTION_TRAITS_H

#include <cstddef>
#include <functional>
#include <tuple>

template<typename>
struct function_traits;

template<typename Ret, typename... Args>
struct function_traits<Ret(Args...)> {
  using type = Ret(Args...);
  using return_type = Ret;
  using argument_types = std::tuple<Args...>;
  static constexpr unsigned arity = sizeof...(Args);
  template<unsigned N>
  using argument_type = std::tuple_element<N, std::tuple<Args...>>::type;
};

template<typename Ret, typename... Args>
struct function_traits<Ret(*)(Args...)> : function_traits<Ret(Args...)> {};

template<typename T, typename Ret, typename... Args>
struct function_traits<Ret(T::*)(Args...)> : function_traits<Ret(Args...)> {};

template<typename T, typename Ret, typename... Args>
struct function_traits<Ret(T::*)(Args...) const>
    : function_traits<Ret(Args...)> {
};

template<typename Functor>
struct function_traits : function_traits<decltype(&Functor::operator())> {};

template<typename T>
using function_traits_t = function_traits<T>::type;

#endif //FUNCTION_TRAITS_H
