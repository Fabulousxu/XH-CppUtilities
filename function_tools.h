#ifndef _XH_FUNCTION_TOOLS_H_
#define _XH_FUNCTION_TOOLS_H_

#include <functional>
#include "function_traits.h"

namespace xh {
using std::function;


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


// getter and setter

template<typename _Ret>
class getter {
	function<_Ret()> _getter;
 public:
	template<typename _F>
	getter(_F &&f) noexcept : _getter(std::forward<_F>(f)) {}
	getter(const getter &) = delete;
	getter(getter &&) = delete;
	getter &operator=(const getter &) = delete;
	getter &operator=(getter &&) = delete;
	operator _Ret() const { return _getter(); }
};

template<typename _Arg>
class setter {
	function<void(_Arg)> _setter;
 public:
	template<typename _F>
	setter(_F &&f) noexcept : _setter(std::forward<_F>(f)) {}
	setter(const setter &) = delete;
	setter(setter &&) = delete;
	setter &operator=(const setter &) = delete;
	setter &operator=(setter &&) = delete;
	void operator=(_Arg arg) { _setter(arg); }
};

#define get(name, return_type) \
const getter<return_type> name = [&]()

#define set(name, argument_type, argument_name) \
const setter<argument_type> name = [&](argument_type argument_name)


// auto return

struct _auto_return_helper {
	template<class T>
	operator T() const { return T{}; }
};

#define auto_return return _auto_return_helper{};


// promise

template<typename>
class Promise;

template<typename _Ret, typename... _Args>
class Promise<_Ret(_Args...)> {
	function<_Ret(_Args...)> _promise;
 public:
	template<typename _F>
	Promise(_F &&f) noexcept: _promise(std::forward<_F>(f)) {}
	_Ret operator()(_Args &&...args) const
	{ return _promise(std::forward<_Args>(args)...); }
	template<typename _F>
	Promise<std::invoke_result_t<_F, _Ret>(_Args...)> then(_F &&f) {
		return {[g = std::forward(f), p = std::move(_promise)](_Args &&...args)
		{ return g(p(std::forward<_Args>(args)...)); }};
	}
};

template<typename _T>
Promise(_T) -> Promise<function_traits_t<_T>>;

}; // namespace xh

#endif //_XH_FUNCTION_TOOLS_H_
