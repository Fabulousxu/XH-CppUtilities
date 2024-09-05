// XH-CppUtilities
// C++20 function_traits.h
// Author: xupeigong@sjtu.edu.cn
// Last Updated: 2024-09-05

#ifndef _XH_FUNCTION_TRAITS_H_
#define _XH_FUNCTION_TRAITS_H_

#include <tuple>
#include <type_traits>

#include "qualifier_traits.h"

namespace xh {

// function check

template <class T>
struct is_function
  : std::is_function<T> {};

template <class T>
struct is_funcptr
  : std::conjunction<
      std::is_pointer<T>, is_function<std::remove_pointer_t<T>>> {};

template <class, class = void>
struct is_functor
  : std::false_type {};

template <class T>
struct is_functor<T, std::void_t<decltype(&T::operator())>>
  : std::true_type {};

template <class T>
struct is_function_or_funcptr
  : is_function<std::remove_pointer_t<T>> {};

template <class T>
struct is_function_or_funcptr_or_functor
  : std::conjunction<is_function_or_funcptr<T>, is_functor<T>> {};

template <class T>
struct is_callable
  : is_function_or_funcptr_or_functor<remove_cvref_t<T>> {};

template <class T>
struct is_memfunc
  : std::is_member_function_pointer<T> {};

template <class T>
inline constexpr bool is_function_v =
  is_function<T>::value;

template <class T>
inline constexpr bool is_funcptr_v =
  is_funcptr<T>::value;

template <class T>
inline constexpr bool is_functor_v =
  is_functor<T>::value;

template <class T>
inline constexpr bool is_function_or_funcptr_v =
  is_function_or_funcptr<T>::value;

template <class T>
inline constexpr bool is_function_or_funcptr_or_functor_v =
  is_function_or_funcptr_or_functor<T>::value;

template <class T>
inline constexpr bool is_callable_v =
  is_callable<T>::value;

template <class T>
inline constexpr bool is_memfunc_v =
  is_memfunc<T>::value;


// function traits

template <class T>
struct _function_traits : _function_traits<function_remove_cvref_t<T>> {};

template <class T>
struct _funcptr_traits : _function_traits<std::remove_pointer_t<T>> {};

template <class T>
struct _functor_traits : _functor_traits<decltype(&T::operator())> {};

template <class T, class C>
struct _functor_traits<T C::*> : _function_traits<T> {};

template <class T>
struct _memfunc_traits {};

template <class Ret, class... Args>
struct _function_traits<Ret(Args...)> {
  using        type = Ret(Args...);
  using return_type = Ret;
  using   arg_tuple = std::tuple<Args...>;
  template <size_t N>
  using    arg_type = std::tuple_element_t<N, arg_tuple>;
  inline static constexpr size_t arity = sizeof...(Args);
};

template <class Ret, class... Args>
struct _function_traits<Ret(Args..., ...)> {
  using        type = Ret(Args..., ...);
  using return_type = Ret;
  using   arg_tuple = std::tuple<Args...>;
  template <size_t N>
  using    arg_type = std::tuple_element_t<N, arg_tuple>;
  inline static constexpr size_t arity = sizeof...(Args);
};

template <class T, class C>
struct _memfunc_traits<T C::*> : _function_traits<T> {
  using             type = T C::*;
  using       class_type = copy_funcqual_add_qualifier_t<T, C>;
  using class_decay_type = C;
  using        func_type = T;
  using  func_decay_type = funcqual_decay_t<T>;
  using       decay_type = func_decay_type C::*;
};

template <class T,
  bool IsFunction = is_function_v<T>, bool IsFuncptr = is_funcptr_v<T>,
  bool IsFunctor  = is_functor_v<T>,  bool IsMemfunc = is_memfunc_v<T>>
struct _function_traits_helper {};

template <class T>
struct _function_traits_helper<T, true, false, false, false>
  : _function_traits<T> {};

template <class T>
struct _function_traits_helper<T, false, true, false, false>
  : _funcptr_traits<T> {};

template <class T>
struct _function_traits_helper<T, false, false, true, false>
  : _functor_traits<T> {};

template <class T>
struct _function_traits_helper<T, false, false, false, true>
  : _memfunc_traits<T> {};

template <class T>
using function_traits = _function_traits_helper<remove_cvref_t<T>>;

template <class T>
using function_traits_t = function_traits<T>::type;

template <class T>
using funcret_t = function_traits<T>::return_type;

template <class T>
using funcarg_tuple = function_traits<T>::arg_tuple;

template <class T, size_t N>
using funcarg_t = function_traits<T>::template arg_type<N>;

template <class T>
inline constexpr size_t funcarity_v = function_traits<T>::arity;

template <class T>
using memfunc_class_t = function_traits<T>::class_type;

template <class T>
using memfunc_class_decay_t = function_traits<T>::class_decay_type;

template <class T>
using memfunc_func_t = function_traits<T>::func_type;

template <class T>
using memfunc_func_decay_t = function_traits<T>::func_decay_type;

template <class T>
using memfunc_decay_t = function_traits<T>::decay_type;

};  // namespace xh

#endif  //_XH_FUNCTION_TRAITS_H_
