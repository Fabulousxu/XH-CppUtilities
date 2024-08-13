#ifndef _XH_FUNCTION_TOOLS_H_
#define _XH_FUNCTION_TOOLS_H_

#include <functional>
#include "function_traits.h"

namespace xh {
using
	std::function,
	std::invoke_result_t,
	std::forward,
	std::is_same_v;

// auto return

struct _auto_return_helper {
	template<class _T>
	operator _T() const { return _T{}; }
};

#define auto_return return _auto_return_helper{};


// to std function

template<typename _T> requires is_callable_v<_T>
inline auto std_function(_T &&func)
{ return function_std_t<_T>(forward<_T>(func)); }


// getter and set

template<typename _Ret>
class get {
	function<_Ret()> _get;
 public:
	template<typename _T> requires (function_arity_v<_T> == 0)
	get(_T &&f) noexcept : _get(f) {}
	get &operator=(const get &) = delete;
	get &operator=(get &&) = delete;
	operator _Ret() const { return _get(); }
};

template<typename _T>
get(_T) -> get<function_return_t<_T>>;


template<typename _Arg>
class set {
	function<void(_Arg)> _set;
 public:
	template<typename _T> requires (function_arity_v<_T> == 1
	  && is_same_v<function_return_t<_T>, void>)
	set(_T &&f) noexcept : _set(f) {}
	set &operator=(const set &) = delete;
	set &operator=(set &&) = delete;
	void operator=(_Arg arg) const { _set(arg); }
};

template<typename _T>
set(_T) -> set<function_argument_t<_T, 0>>;


template<typename _Ret, typename _Arg>
class getset {
	function<_Ret()> _get;
	function<void(_Arg)> _set;
 public:
	template<typename _T1, typename _T2> requires (function_arity_v<_T1> == 0
	  && function_arity_v<_T2> == 1 && is_same_v<function_return_t<_T2>, void>)
	getset(_T1 &&f1, _T2 &&f2) noexcept : _get(f1), _set(f2) {}
	getset &operator=(const getset &) = delete;
	getset &operator=(getset &&) = delete;
	operator _Ret() const { return _get(); }
	void operator=(_Arg arg) const { _set(arg); }
};

template<typename _T1, typename _T2>
getset(_T1, _T2) -> getset<function_return_t<_T1>, function_argument_t<_T2, 0>>;


// member function

template<typename>
class member_function;

#define MEMBER_FUNCTION_QUALIFIERS(qualifiers)                           \
template <typename _Ret, typename _Cls, typename... _Args>               \
class member_function<_Ret(_Cls::*)(_Args...)qualifiers> {               \
  _Ret(_Cls::*_member_function)(_Args...)qualifiers;                     \
 public:                                                                 \
  template<typename _F>                                                  \
  member_function(_F &&f) noexcept : _member_function(forward<_F>(f)) {} \
	_Ret operator()(qualifiers _Cls &obj, _Args... args) const             \
	{ return (obj.*_member_function)(args...); }                           \
  _Ret operator()(qualifiers _Cls *obj, _Args... args) const             \
	{ return (obj->*_member_function)(args...); }                          \
};

MEMBER_FUNCTION_QUALIFIERS()
MEMBER_FUNCTION_QUALIFIERS(const)
MEMBER_FUNCTION_QUALIFIERS(volatile)
MEMBER_FUNCTION_QUALIFIERS(const volatile)

#undef MEMBER_FUNCTION_QUALIFIERS

template<typename _T>
member_function(_T) -> member_function<std::remove_cvref_t<_T>>;


// promise

template<typename>
class promise;

template<typename _Ret, typename... _Args>
class promise<_Ret(_Args...)> {
	function<_Ret(_Args...)> _promise;
 public:
	template<typename _T>
	promise(_T &&f) noexcept: _promise(f) {}
	_Ret operator()(_Args &&...args) const
	{ return _promise(forward<_Args>(args)...); }
	template<typename _F>
	promise<function_return_t<_F>(_Args...)> then(_F &&f)  {
		return {[f, this](_Args &&...args)
		{ return f(_promise(forward<_Args>(args)...)); }};
	}
};

template<typename _T>
promise(_T) -> promise<function_traits_t<_T>>;

}; // namespace xh

#endif //_XH_FUNCTION_TOOLS_H_
