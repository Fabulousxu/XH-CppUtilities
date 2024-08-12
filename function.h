#ifndef _XH_FUNCTION_H_
#define _XH_FUNCTION_H_

#include <tuple>
#include <type_traits>

namespace xh {

using std::bool_constant, std::false_type, std::true_type, std::remove_cvref_t,
	std::remove_pointer_t, std::void_t, std::is_member_function_pointer_v,
	std::is_function_v, std::tuple, std::tuple_element_t;

template<typename _T>
inline constexpr bool
	is_function_pointer_v = is_function_v<remove_pointer_t<_T>>;
template<typename _T>
struct is_function_pointer : bool_constant<is_function_pointer_v<_T>> {};

template<typename, typename = void>
struct is_functor : false_type {};
template<typename _T>
struct is_functor<_T, void_t<decltype(&_T::operator())>> : true_type {};
template<typename _T>
inline constexpr bool is_functor_v = is_functor<_T>::value;

template<typename _T,
	bool _Is = is_function_v<remove_pointer_t<_T>> || is_functor_v<_T>>
struct _is_callable_helper : bool_constant<_Is> {};
template<typename _T>
struct is_callable : _is_callable_helper<remove_cvref_t<_T>> {};
template<typename _T>
inline constexpr bool is_callable_v = is_callable<_T>::value;

template<typename _T> requires is_member_function_pointer_v<_T>
struct remove_mem_fun_const {};
template<typename _T> requires is_member_function_pointer_v<_T>
struct remove_mem_fun_volatile {};
template<typename _T> requires is_member_function_pointer_v<_T>
struct remove_mem_fun_cv {};

#define REMOVE_MEM_FUN_QUALIFIERS(remove, qualifiers, result)               \
template<typename _Ret, typename _Class, typename... _Args>                 \
struct remove_mem_fun_##remove<_Ret(_Class::*)(_Args...) qualifiers> {      \
  using type = _Ret(_Class::*)(_Args...) result;                            \
};                                                                          \
template<typename _Ret, typename _Class, typename... _Args>                 \
struct remove_mem_fun_##remove<_Ret(_Class::*)(_Args..., ...) qualifiers> { \
  using type = _Ret(_Class::*)(_Args...) result;                            \
};
REMOVE_MEM_FUN_QUALIFIERS(const, ,)
REMOVE_MEM_FUN_QUALIFIERS(const, const,)
REMOVE_MEM_FUN_QUALIFIERS(const, volatile, volatile)
REMOVE_MEM_FUN_QUALIFIERS(const, const volatile, volatile)
REMOVE_MEM_FUN_QUALIFIERS(volatile, ,)
REMOVE_MEM_FUN_QUALIFIERS(volatile, const, const)
REMOVE_MEM_FUN_QUALIFIERS(volatile, volatile,)
REMOVE_MEM_FUN_QUALIFIERS(volatile, const volatile, const)
REMOVE_MEM_FUN_QUALIFIERS(cv, ,)
REMOVE_MEM_FUN_QUALIFIERS(cv, const,)
REMOVE_MEM_FUN_QUALIFIERS(cv, volatile,)
REMOVE_MEM_FUN_QUALIFIERS(cv, const volatile,)
#undef REMOVE_MEM_FUN_QUALIFIERS

template<typename _T>
using remove_mem_fun_const_t = remove_mem_fun_const<_T>::type;
template<typename _T>
using remove_mem_fun_volatile_t = remove_mem_fun_volatile<_T>::type;
template<typename _T>
using remove_mem_fun_cv_t = remove_mem_fun_cv<_T>::type;

template<typename _T>
struct _remove_mem_fun_class_helper {};
template<typename _T, typename _C>
struct _remove_mem_fun_class_helper<_T(_C::*)> { using type = _T; };
template<typename _T> requires is_member_function_pointer_v<_T>
struct remove_mem_fun_class : _remove_mem_fun_class_helper<
	remove_mem_fun_cv_t<_T>> {
};
template<typename _T>
using remove_mem_fun_class_t = remove_mem_fun_class<_T>::type;

template<typename _T>
struct _callable_traits_base {};
template<typename _Ret, typename... _Args>
struct _callable_traits_base<_Ret(_Args...)> {
  using type = _Ret(_Args...);
  using return_type = _Ret;
  inline static constexpr unsigned arity = sizeof...(_Args);
  using argument_tuple = tuple<_Args...>;
  template<unsigned _Idx>
  using argument_type = tuple_element_t<_Idx, argument_tuple>;
};
template<typename _Ret, typename... _Args>
struct _callable_traits_base<_Ret(_Args..., ...)> {
  using type = _Ret(_Args..., ...);
  using return_type = _Ret;
};
template<typename _T, typename = void>
struct _callable_traits_helper : _callable_traits_base<_T> {};
template<typename _T>
struct _callable_traits_helper<_T, void_t<decltype(&_T::operator())>>
	: _callable_traits_base<remove_mem_fun_class_t<
		decltype(&_T::operator())>> {
};
template<typename _T> requires is_callable_v<_T>
struct callable_traits : _callable_traits_helper<
	remove_pointer_t<remove_cvref_t<_T>>> {
};
template<typename _T>
using callable_traits_t = callable_traits<_T>::type;
template<typename _T>
using callable_return_t = callable_traits<_T>::return_type;
template<typename _T>
inline constexpr unsigned callable_arity_v = callable_traits<_T>::arity;
template<typename _T, unsigned _Idx>
using callable_argument_t = callable_traits<_T>::template argument_type<_Idx>;
template<typename _T>
using callable_argument_tuple = callable_traits<_T>::argument_tuple;

template<typename>
class mem_fun;

#define MEM_FUN(cv)                                                \
  template <typename Ret, typename Class, typename... Args>        \
  class mem_fun<Ret (Class::*)(Args...) cv> {                      \
    Ret (Class::*_mem_fun)(Args...) cv;                            \
                                                                   \
   public:                                                         \
    mem_fun(Ret (Class::*f)(Args...) cv) noexcept : _mem_fun(f) {} \
    Ret operator()(cv Class &obj, Args... args) const {            \
      return (obj.*_mem_fun)(args...);                             \
    }                                                              \
    Ret operator()(cv Class *obj, Args... args) const {            \
      return (obj->*_mem_fun)(args...);                            \
    }                                                              \
  };

MEM_FUN()
MEM_FUN(const)
MEM_FUN(volatile)
MEM_FUN(const volatile)
#undef MEM_FUN

template<typename T>
mem_fun(T) -> mem_fun<std::remove_cvref_t<T>>;

};  // namespace xh

#endif  //_XH_FUNCTION_H_
