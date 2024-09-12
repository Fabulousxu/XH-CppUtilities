// XH-CppUtilities
// C++20 function_traits.h
// Author: xupeigong@sjtu.edu.cn
// Last Updated: 2024-09-12

#ifndef _XH_FUNCTION_TRAITS_H_
#define _XH_FUNCTION_TRAITS_H_

#include <tuple>
#include <type_traits>

#include "qualifier.h"

namespace xh {

using namespace std;

template <class T>
struct is_function : std::is_function<T> {};

template <class T>
struct is_funcptr
  : conjunction<is_pointer<T>, is_function<remove_pointer_t<T>>> {};

template <class, class = void>
struct is_functor : false_type {};

template <class T>
struct is_functor<T, void_t<decltype(&T::operator())>> : true_type {};

template <class T>
struct is_memfunc : is_member_function_pointer<T> {};

template <class T>
inline constexpr bool is_function_v = is_function<T>::value;
template <class T>
inline constexpr bool is_funcptr_v = is_funcptr<T>::value;
template <class T>
inline constexpr bool is_functor_v = is_functor<T>::value;
template <class T>
inline constexpr bool is_memfunc_v = is_memfunc<T>::value;

// nth_of

template <size_t N, class... T>
struct nth_of : tuple_element<N, tuple<T...>> {};

template <size_t N, class... T>
using nth_of_t = nth_of<N, T...>::type;

// function traits

template <class T>
struct _function_traits : _function_traits<funcqual_decay_t<T>> {};

template <class T>
struct _funcptr_traits : _function_traits<remove_pointer_t<T>> {};

template <class T>
struct _functor_traits : _functor_traits<decltype(&T::operator())> {};

template <class T, class C>
struct _functor_traits<T C::*> : _function_traits<T> {};

template <class T>
struct _memfunc_traits;

template <class Ret, class... Args>
struct _function_traits<Ret(Args...)> {
  using        type = Ret(Args...);
  using return_type = Ret;
  using   arg_tuple = tuple<Args...>;
  template <size_t N>
  using    arg_type = nth_of_t<N, Args...>;
  inline static constexpr size_t arity = sizeof...(Args);
};

template <class Ret, class... Args>
struct _function_traits<Ret(Args..., ...)> {
  using        type = Ret(Args..., ...);
  using return_type = Ret;
  using   arg_tuple = tuple<Args...>;
  template <size_t N>
  using    arg_type = nth_of_t<N, Args...>;
  inline static constexpr size_t arity = sizeof...(Args);
};

template <class T, class C>
struct _memfunc_traits<T C::*> : _function_traits<T> {
  using          type = T C::*;
  using    class_type = C;
  using function_type = T;
};

template <class T>
struct _function_traits_helper
  : conditional_t<is_function_v<T>, _function_traits<T>,
      conditional_t<is_funcptr_v<T>, _funcptr_traits<T>,
        conditional_t<is_functor_v<T>, _functor_traits<T>,
          conditional_t<is_memfunc_v<T>, _memfunc_traits<T>, false_type>>>> {};

template <class T>
using function_traits = _function_traits_helper<decay_t<T>>;

template <class T>
using function_traits_t = function_traits<T>::type;
template <class T>
using functraits_t = function_traits<T>::type;
template <class T>
using function_return_t = function_traits<T>::return_type;
template <class T>
using funcret_t = function_traits<T>::return_type;
template <class T>
using function_arg_tuple = function_traits<T>::arg_tuple;
template <class T>
using funcarg_tuple = function_traits<T>::arg_tuple;
template <class T, size_t N>
using function_arg_t = function_traits<T>::template arg_type<N>;
template <class T, size_t N>
using funcarg_t = function_traits<T>::template arg_type<N>;
template <class T>
inline constexpr size_t function_arity_v = function_traits<T>::arity;
template <class T>
inline constexpr size_t funcarity_v = function_traits<T>::arity;
template <class T>
using memfunc_class_t = function_traits<T>::class_type;
template <class T>
using mfclass_t = function_traits<T>::class_type;
template <class T>
using memfunc_function_t = function_traits<T>::function_type;
template <class T>
using mffunction_t = function_traits<T>::function_type;

}; // namespace xh

#endif //_XH_FUNCTION_TRAITS_H_
