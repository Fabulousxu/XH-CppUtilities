#ifndef _XH_FUNCTION_TRAITS_H_
#define _XH_FUNCTION_TRAITS_H_

#include <tuple>
#include <type_traits>

namespace xh {
using
	std::bool_constant,
	std::false_type,
	std::true_type,
	std::void_t,
	std::remove_cvref_t,
	std::remove_pointer_t,
	std::tuple,
	std::tuple_element_t,
	std::is_member_function_pointer_v,
	std::is_function_v;


// basic function traits

template<typename _T>
inline constexpr bool is_function_pointer_v
	= is_function_v<remove_pointer_t<_T>>;

template<typename _T>
struct is_function_pointer
	: bool_constant<is_function_pointer_v<_T>> {};


template<typename _T>
inline constexpr bool is_function_or_pointer_v
	= is_function_v<_T> || is_function_pointer_v<_T>;

template<typename _T>
struct is_function_or_pointer
	: bool_constant<is_function_or_pointer_v<_T>> {};


template<typename, typename = void>
struct is_functor
	: false_type {};

template<typename _T>
struct is_functor<_T, void_t<decltype(&_T::operator())>>
	: true_type {};

template<typename _T>
inline constexpr bool is_functor_v
	= is_functor<_T>::value;


template<typename _T,
	bool _Is = is_function_or_pointer_v<_T> || is_functor_v<_T>>
struct _is_callable_helper
	: bool_constant<_Is> {};

template<typename _T>
struct is_callable
	: _is_callable_helper<remove_cvref_t<_T>> {};

template<typename _T>
inline constexpr bool is_callable_v
	= is_callable<_T>::value;


// member function traits

template<typename _T>
struct remove_member_function_const {};

template<typename _T>
struct remove_member_function_volatile {};

template<typename _T>
struct remove_member_function_cv {};

#define REMOVE_MEMBER_FUNCTION_QUALIFIERS(remove, qualifiers, result)          \
template<typename _Ret, typename _Cls, typename... _Args>                      \
struct remove_member_function_##remove<_Ret(_Cls::*)(_Args...)qualifiers>      \
	{ using type = _Ret(_Cls::*)(_Args...) result; };                            \
                                                                               \
template<typename _Ret, typename _Cls, typename... _Args>                      \
struct remove_member_function_##remove<_Ret(_Cls::*)(_Args..., ...)qualifiers> \
	{ using type = _Ret(_Cls::*)(_Args..., ...) result; };

REMOVE_MEMBER_FUNCTION_QUALIFIERS(const, ,)
REMOVE_MEMBER_FUNCTION_QUALIFIERS(const, const,)
REMOVE_MEMBER_FUNCTION_QUALIFIERS(const, volatile, volatile)
REMOVE_MEMBER_FUNCTION_QUALIFIERS(const, const volatile, volatile)
REMOVE_MEMBER_FUNCTION_QUALIFIERS(volatile, ,)
REMOVE_MEMBER_FUNCTION_QUALIFIERS(volatile, const, const)
REMOVE_MEMBER_FUNCTION_QUALIFIERS(volatile, volatile,)
REMOVE_MEMBER_FUNCTION_QUALIFIERS(volatile, const volatile, const)
REMOVE_MEMBER_FUNCTION_QUALIFIERS(cv, ,)
REMOVE_MEMBER_FUNCTION_QUALIFIERS(cv, const,)
REMOVE_MEMBER_FUNCTION_QUALIFIERS(cv, volatile,)
REMOVE_MEMBER_FUNCTION_QUALIFIERS(cv, const volatile,)

#undef REMOVE_MEMBER_FUNCTION_QUALIFIERS

template<typename _T>
using remove_member_function_const_t
	= remove_member_function_const<_T>::type;

template<typename _T>
using remove_member_function_volatile_t
	= remove_member_function_volatile<_T>::type;

template<typename _T>
using remove_member_function_cv_t
	= remove_member_function_cv<_T>::type;


template<typename _T>
struct _remove_member_function_class_helper {};

template<typename _T, typename _C>
struct _remove_member_function_class_helper<_T(_C::*)>
  { using type = _T; };

template<typename _T>
struct remove_member_function_class
	: _remove_member_function_class_helper<remove_member_function_cv_t<_T>> {};

template<typename _T>
using remove_member_function_class_t
	= remove_member_function_class<_T>::type;


// callable traits

template<typename _T>
struct _callable_traits_base {};

template<typename _Ret, typename... _Args>
struct _callable_traits_base<_Ret(_Args...)> {
  using type = _Ret(_Args...);
  using return_type = _Ret;
	using argument_tuple = tuple<_Args...>;
	template<unsigned _Idx>
	using argument_type = tuple_element_t<_Idx, argument_tuple>;
	inline static constexpr unsigned arity = sizeof...(_Args);
};

template<typename _Ret, typename... _Args>
struct _callable_traits_base<_Ret(_Args..., ...)> {
  using type = _Ret(_Args..., ...);
  using return_type = _Ret;
};

template<typename _T, bool _Is = is_function_or_pointer_v<_T>>
struct _callable_traits_helper {};

template<typename _T>
struct _callable_traits_helper<_T, true>
	: _callable_traits_base<remove_pointer_t<_T>> {};

template<typename _T>
struct _callable_traits_helper<_T, false>
	: _callable_traits_base<
			remove_member_function_class_t<decltype(&_T::operator())>> {};

template<typename _T>
struct callable_traits
	: _callable_traits_helper<remove_cvref_t<_T>> {};

template<typename _T>
using callable_traits_t
	= callable_traits<_T>::type;

template<typename _T>
using callable_return_t
	= callable_traits<_T>::return_type;

template<typename _T>
using callable_argument_tuple
	= callable_traits<_T>::argument_tuple;

template<typename _T, unsigned _Idx>
using callable_argument_t
	= callable_traits<_T>::template argument_type<_Idx>;

template<typename _T>
inline constexpr unsigned callable_arity_v
	= callable_traits<_T>::arity;


// function traits



// member function

template<typename>
class member_function;

#define MEMBER_FUNCTION_QUALIFIERS(qualifiers)                  \
template <typename _Ret, typename _Cls, typename... _Args>      \
class member_function<_Ret(_Cls::*)(_Args...)qualifiers> {      \
  _Ret(_Cls::*_member_function)(_Args...)qualifiers;            \
 public:                                                        \
  member_function(_Ret(_Cls::* f)(_Args...)qualifiers) noexcept \
		: _member_function(f) {}                                    \
	_Ret operator()(qualifiers _Cls &obj, _Args... args) const    \
	{ return (obj.*_member_function)(args...); }                  \
  _Ret operator()(qualifiers _Cls *obj, _Args... args) const    \
	{ return (obj->*_member_function)(args...); }                 \
};

MEMBER_FUNCTION_QUALIFIERS()
MEMBER_FUNCTION_QUALIFIERS(const)
MEMBER_FUNCTION_QUALIFIERS(volatile)
MEMBER_FUNCTION_QUALIFIERS(const volatile)

#undef MEMBER_FUNCTION_QUALIFIERS

template<typename T>
member_function(T) -> member_function<std::remove_cvref_t<T>>;

};  // namespace xh

#endif  //_XH_FUNCTION_TRAITS_H_
