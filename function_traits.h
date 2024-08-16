#ifndef _XH_FUNCTION_TRAITS_H_
#define _XH_FUNCTION_TRAITS_H_

#include <tuple>
#include <type_traits>

using namespace std;

namespace xh {

// is function pointer

template<class T>
inline constexpr bool is_function_pointer_v =
	is_pointer_v<T> && is_function_v<remove_pointer_t<T>>;

template<class T>
struct is_function_pointer : bool_constant<is_function_pointer_v<T>> {};


// is function or pointer

template<class T>
inline constexpr bool is_function_or_pointer_v =
	is_function_v<remove_pointer_t<T>>;

template<class T>
struct is_function_or_pointer : bool_constant<is_function_or_pointer_v<T>> {};


// is functor

template<class, class = void>
struct is_functor : false_type {};

template<class T>
struct is_functor<T, void_t<decltype(&T::operator())>> : true_type {};

template<class T>
inline constexpr bool is_functor_v = is_functor<T>::value;


// member function traits

template<class T> requires is_member_function_pointer_v<remove_cvref_t<T>>
struct member_function_traits : member_function_traits<remove_cvref_t<T>> {};

#define _MEMBER_FUNCTION_TRAITS(cv, ref, ...)                           \
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


// is member function cvref

template<class T> requires is_member_function_pointer_v<T>
struct is_member_function_const
	: is_const<typename member_function_traits<T>::class_type> {};

template<class T> requires is_member_function_pointer_v<T>
struct is_member_function_volatile
	: is_volatile<typename member_function_traits<T>::class_type> {};

template<class T> requires is_member_function_pointer_v<T>
struct is_member_function_reference
	: is_reference<typename member_function_traits<T>::class_type> {};

template<class T> requires is_member_function_pointer_v<T>
struct is_member_function_lvalue_reference
	: is_lvalue_reference<typename member_function_traits<T>::class_type> {};

template<class T> requires is_member_function_pointer_v<T>
struct is_member_function_rvalue_reference
	: is_rvalue_reference<typename member_function_traits<T>::class_type> {};

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


// remove member function cvref

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

#define _REMOVE_MEMBER_FUNCTION(name, cvref, result)                 \
  template <class R, class C, class... Args>                         \
  struct remove_member_function_##name<R (C::*)(Args...) cvref>      \
	{ using type = R (C::*)(Args...) result; };                        \
  template <class R, class C, class... Args>                         \
  struct remove_member_function_##name<R (C::*)(Args..., ...) cvref> \
	{ using type = R (C::*)(Args..., ...) result; };

#define __REMOVE_MEMBER_FUNCTION(name, _, _lref, _rref, _const, _const_lref, \
                                 _const_rref, _volatile, _volatile_lref,     \
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


// remove member function class

template<class T> requires is_member_function_pointer_v<T>
struct remove_member_function_class {};

template<class T, class C>
struct remove_member_function_class<T C::*> { using type = T; };

template<class T>
using remove_member_function_class_t = remove_member_function_class<T>::type;

template<class T> requires is_member_function_pointer_v<T>
struct remove_member_function_cvref_class
	: remove_member_function_class<remove_member_function_cvref_t<T>> {};

template<class T>
using remove_member_function_cvref_class_t =
	remove_member_function_cvref_class<T>::type;


// non member function traits

template<class T>
struct _non_member_function_traits
	: _non_member_function_traits<
		remove_member_function_cvref_class_t<decltype(&T::operator())>> {};

#define _NON_MEMBER_FUNCTION_TRAITS(...)                       \
  template <class R, class... Args>                            \
  struct _non_member_function_traits<R(Args... __VA_ARGS__)> { \
    using type = R(Args... __VA_ARGS__);                       \
    using return_type = R;                                     \
    using argument_tuple = tuple<Args...>;                     \
    template <size_t N>                                        \
    using argument_type = tuple_element_t<N, argument_tuple>;  \
    inline static constexpr size_t arity = sizeof...(Args);    \
  };

_NON_MEMBER_FUNCTION_TRAITS()
_NON_MEMBER_FUNCTION_TRAITS(, ...)

#undef _NON_MEMBER_FUNCTION_TRAITS

template<class T>
	requires is_function_or_pointer_v<remove_cvref_t<T>> ||
	         is_functor_v<remove_cvref_t<T>>
struct non_member_function_traits
	: _non_member_function_traits<remove_pointer_t<remove_cvref_t<T>>> {};


// function traits

template<class T, bool Is = is_member_function_pointer_v<remove_cvref_t<T>>>
	requires is_function_or_pointer_v<remove_cvref_t<T>> ||
	         is_functor_v<remove_cvref_t<T>> ||
					 is_member_function_pointer_v<remove_cvref_t<T>>
struct function_traits : non_member_function_traits<T> {};

template<class T>
struct function_traits<T, true> : member_function_traits<T> {};

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
