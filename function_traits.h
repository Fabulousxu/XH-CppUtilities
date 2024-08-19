// C++20 function_traits.h
// Author: xupeigong@sjtu.edu.cn
// Last Updated: 2024-08-19
//
// This header file defines a series of function type traits, expanding on the
// type traits not fully provided by the standard library for function types.
// It includes utilities for checking callable types, such as function pointers
// and functors, and for checking and removing qualifiers of member functions.
// Most mainly, it offers function traits for callable types and member
// functions, allowing users to get function return types, argument types, and
// the class type of member functions. This function traits supports variadic
// functions and recognizes and erases cv-qualifiers and reference-qualifiers
// from any callable types and member functions.

#ifndef _XH_FUNCTION_TRAITS_H_
#define _XH_FUNCTION_TRAITS_H_

#include <tuple>
#include <type_traits>

using namespace std;

namespace xh {

/************************
 * check if is callable *
 ************************/

template<class T>
struct is_function_pointer
	: bool_constant<is_pointer_v<T> && is_function_v<remove_pointer_t<T>>> {};

template<class T>
struct is_function_or_pointer : is_function<remove_pointer_t<T>> {};

template<class, class = void>
struct is_functor : false_type {};

template<class T>
struct is_functor<T, void_t<decltype(&T::operator())>> : true_type {};

template<class T>
struct is_function_or_pointer_or_functor
	: bool_constant<is_function_or_pointer<T>::value || is_functor<T>::value> {};

template<class T>
struct is_callable : is_function_or_pointer_or_functor<remove_cvref_t<T>> {};

// _t aliases and _v variables

template<class T>
inline constexpr bool is_function_pointer_v = is_function_pointer<T>::value;

template<class T>
inline constexpr bool is_function_or_pointer_v =
	is_function_or_pointer<T>::value;

template<class T>
inline constexpr bool is_functor_v = is_functor<T>::value;

template<class T>
inline constexpr bool is_function_or_pointer_or_functor_v =
	is_function_or_pointer_or_functor<T>::value;

template<class T>
inline constexpr bool is_callable_v = is_callable<T>::value;


/**************************
 * member function traits *
 **************************/

template<class T> requires is_member_function_pointer_v<remove_cvref_t<T>>
struct member_function_traits	: member_function_traits<remove_cvref_t<T>> {};

#define _MEMBER_FUNCTION_TRAITS(cv, ref, ...)                   \
  template <class R, class C, class... Args>                            \
  struct member_function_traits<R (C::*)(Args... __VA_ARGS__) cv ref> { \
    using type = R (C::*)(Args... __VA_ARGS__) cv ref;                  \
    using class_type = cv C ref;                                        \
    using return_type = R;                                              \
    using argument_tuple = tuple<Args...>;                              \
    template <size_t N>                                                 \
    using argument_type = tuple_element_t<N, argument_tuple>;           \
    inline static constexpr size_t arity = sizeof...(Args);             \
  };

#define __MEMBER_FUNCTION_TRAITS(cv)    \
  _MEMBER_FUNCTION_TRAITS(cv, )         \
  _MEMBER_FUNCTION_TRAITS(cv, , , ...)  \
  _MEMBER_FUNCTION_TRAITS(cv, &)        \
  _MEMBER_FUNCTION_TRAITS(cv, &, , ...) \
  _MEMBER_FUNCTION_TRAITS(cv, &&)       \
  _MEMBER_FUNCTION_TRAITS(cv, &&, , ...)

__MEMBER_FUNCTION_TRAITS()
__MEMBER_FUNCTION_TRAITS(const)
__MEMBER_FUNCTION_TRAITS(volatile)
__MEMBER_FUNCTION_TRAITS(const volatile)

#undef _MEMBER_FUNCTION_TRAITS
#undef __MEMBER_FUNCTION_TRAITS

// _t aliases and _v variables

template<class T>
using member_function_traits_t = member_function_traits<T>::type;

template<class T>
using member_function_class_t = member_function_traits<T>::class_type;

template<class T>
using member_function_return_t = member_function_traits<T>::return_type;

template<class T>
using member_function_argument_tuple =
	member_function_traits<T>::argument_tuple;

template<class T, size_t N>
using member_function_argument_t =
	member_function_traits<T>::template argument_type<N>;

template<class T>
inline constexpr size_t member_function_arity_v =
	member_function_traits<T>::arity;


/******************************************
 * check if member function has qualifier *
 ******************************************/

template<class T> requires is_member_function_pointer_v<T>
struct is_member_function_const : is_const<member_function_class_t<T>> {};

template<class T> requires is_member_function_pointer_v<T>
struct is_member_function_volatile : is_volatile<member_function_class_t<T>> {};

template<class T> requires is_member_function_pointer_v<T>
struct is_member_function_reference
	: is_reference<member_function_class_t<T>> {};

template<class T> requires is_member_function_pointer_v<T>
struct is_member_function_lvalue_reference
	: is_lvalue_reference<member_function_class_t<T>> {};

template<class T> requires is_member_function_pointer_v<T>
struct is_member_function_rvalue_reference
	: is_rvalue_reference<member_function_class_t<T>> {};

// _t aliases and _v variables

template<class T>
inline constexpr bool is_member_function_const_v =
	is_member_function_const<T>::value;

template<class T>
inline constexpr bool is_member_function_volatile_v =
	is_member_function_volatile<T>::value;

template<class T>
inline constexpr bool is_member_function_reference_v =
	is_member_function_reference<T>::value;

template<class T>
inline constexpr bool is_member_function_lvalue_reference_v =
	is_member_function_lvalue_reference<T>::value;

template<class T>
inline constexpr bool is_member_function_rvalue_reference_v =
	is_member_function_rvalue_reference<T>::value;


/************************************************
 * remove qualifier or class of member function *
 ************************************************/

template<class T> requires is_member_function_pointer_v<T>
struct remove_member_function_reference {};

template<class T> requires is_member_function_pointer_v<T>
struct remove_member_function_const {};

template<class T> requires is_member_function_pointer_v<T>
struct remove_member_function_volatile {};

template<class T> requires is_member_function_pointer_v<T>
struct remove_member_function_cv {};

template<class T> requires is_member_function_pointer_v<T>
struct remove_member_function_cvref {};

#define _REMOVE_MEMBER_FUNCTION(name, cvref, result)                   \
  template <class R, class C, class... Args>                           \
  struct remove_member_function_##name<R (C::*)(Args...) cvref> {      \
    using type = R (C::*)(Args...) result;                             \
  };                                                                   \
  template <class R, class C, class... Args>                           \
  struct remove_member_function_##name<R (C::*)(Args..., ...) cvref> { \
    using type = R (C::*)(Args..., ...) result;                        \
  };

#define __REMOVE_MEMBER_FUNCTION(name, _, _lref, _rref, _const, _const_lref, \
                                 _const_rref, _volatile, _volatile_lref, \
                                 _volatile_rref, _cv, _cv_lref, _cv_rref)    \
  _REMOVE_MEMBER_FUNCTION(name, , _)                                         \
  _REMOVE_MEMBER_FUNCTION(name, &, _lref)                                    \
  _REMOVE_MEMBER_FUNCTION(name, &&, _rref)                                   \
  _REMOVE_MEMBER_FUNCTION(name, const, _const)                               \
  _REMOVE_MEMBER_FUNCTION(name, const &, _const_lref)                        \
  _REMOVE_MEMBER_FUNCTION(name, const &&, _const_rref)                       \
  _REMOVE_MEMBER_FUNCTION(name, volatile, _volatile)                         \
  _REMOVE_MEMBER_FUNCTION(name, volatile &, _volatile_lref)                  \
  _REMOVE_MEMBER_FUNCTION(name, volatile &&, _volatile_rref)                 \
  _REMOVE_MEMBER_FUNCTION(name, const volatile, _cv)                         \
  _REMOVE_MEMBER_FUNCTION(name, const volatile &, _cv_lref)                  \
  _REMOVE_MEMBER_FUNCTION(name, const volatile &&, _cv_rref)

__REMOVE_MEMBER_FUNCTION(reference, , , , const, const, const, volatile,
	volatile, volatile, const volatile, const volatile, const volatile)
__REMOVE_MEMBER_FUNCTION(const, , &, &&, , const &, const &&, volatile,
	volatile &, volatile &&, volatile, const volatile &, const volatile &&)
__REMOVE_MEMBER_FUNCTION(volatile, , &, &&, const, const &, const &&, ,
	volatile &, volatile &&, const, const volatile &, const volatile &&)
__REMOVE_MEMBER_FUNCTION(cv, , &, &&, , const &, const &&, , volatile &,
	volatile &&, , const volatile &, const volatile &&)
__REMOVE_MEMBER_FUNCTION(cvref, , , , , , , , , , , ,)

#undef _REMOVE_MEMBER_FUNCTION
#undef __REMOVE_MEMBER_FUNCTION

template<class T> requires is_member_function_pointer_v<T>
struct remove_member_function_class {};

template<class T, class C>
struct remove_member_function_class<T C::*> { using type = T; };

template<class T> requires is_member_function_pointer_v<T>
struct remove_member_function_cvref_class
	: remove_member_function_class<
		typename remove_member_function_cvref<T>::type> {};

// _t aliases and _v variables

template<class T>
using remove_member_function_reference_t =
	remove_member_function_reference<T>::type;

template<class T>
using remove_member_function_const_t = remove_member_function_const<T>::type;

template<class T>
using remove_member_function_volatile_t =
	remove_member_function_volatile<T>::type;

template<class T>
using remove_member_function_cv_t = remove_member_function_cv<T>::type;

template<class T>
using remove_member_function_cvref_t = remove_member_function_cvref<T>::type;

template<class T>
using remove_member_function_class_t = remove_member_function_class<T>::type;

template<class T>
using remove_member_function_cvref_class_t =
	remove_member_function_cvref_class<T>::type;


/*******************
 * callable traits *
 *******************/

template<class T> requires is_function_or_pointer_or_functor_v<T>
struct function_or_pointer_or_functor_traits
	: function_or_pointer_or_functor_traits<
		remove_member_function_cvref_class_t<decltype(&T::operator())>> {};

#define _FUNCTION_OR_POINTER_OR_FUNCTOR_TRAITS(...)                      \
  template <class R, class... Args>                                      \
  struct function_or_pointer_or_functor_traits<R(Args... __VA_ARGS__)> { \
    using type = R(Args... __VA_ARGS__);                                 \
    using return_type = R;                                               \
    using argument_tuple = tuple<Args...>;                               \
    template <size_t N>                                                  \
    using argument_type = tuple_element_t<N, argument_tuple>;            \
    inline static constexpr size_t arity = sizeof...(Args);              \
  };

_FUNCTION_OR_POINTER_OR_FUNCTOR_TRAITS()
_FUNCTION_OR_POINTER_OR_FUNCTOR_TRAITS(, ...)

#undef _FUNCTION_OR_POINTER_OR_FUNCTOR_TRAITS

template<class T> requires is_callable_v<T>
struct callable_traits : function_or_pointer_or_functor_traits<
	remove_pointer_t<remove_cvref_t<T>>> {};

// _t aliases and _v variables

template<class T>
using callable_traits_t = callable_traits<T>::type;

template<class T>
using callable_return_t = callable_traits<T>::return_type;

template<class T>
using callable_argument_tuple = callable_traits<T>::argument_tuple;

template<class T, size_t N>
using callable_argument_t = callable_traits<T>::template argument_type<N>;

template<class T>
inline constexpr size_t callable_arity_v = callable_traits<T>::arity;


/*******************
 * function traits *
 *******************/

template<class T, bool Is = is_callable_v<T>>
  requires is_callable_v<T> || is_member_function_pointer_v<remove_cvref_t<T>>
struct function_traits : callable_traits<T> {};

template<class T>
struct function_traits<T, false> : member_function_traits<T> {};

// _t aliases and _v variables

template<class T>
using function_traits_t = function_traits<T>::type;

template<class T>
using function_class_t = function_traits<T>::class_type;

template<class T>
using function_return_t = function_traits<T>::return_type;

template<class T>
using function_argument_tuple = function_traits<T>::argument_tuple;

template<class T, size_t N>
using function_argument_t = function_traits<T>::template argument_type<N>;

template<class T>
inline constexpr size_t function_arity_v = function_traits<T>::arity;

};  // namespace xh

#endif  //_XH_FUNCTION_TRAITS_H_
