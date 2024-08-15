#ifndef _XH_FUNCTION_TOOLS_H_
#define _XH_FUNCTION_TOOLS_H_

#include <functional>
#include "function_traits.h"

using namespace std;

namespace xh {

// auto return

struct _auto_return {
	template<class T>
	operator T() const { return T{}; }
};

#define auto_return return _auto_return{};


// getter and setter

template<class R>
class get {
	function<R()> _get;
 public:
	template<class T> requires (function_arity_v<T> == 0)
	get(T &&f) noexcept : _get(forward<T>(f)) {}
	get &operator=(const get &) = delete;
	get &operator=(get &&) = delete;
	operator R() const { return _get(); }
};

template<class T>
get(T) -> get<function_return_t<T>>;

template<class Arg>
class set {
	function<void(Arg)> _set;
 public:
	template<class T> requires (function_arity_v<T> == 1
	  && is_void_v<function_return_t<T>>)
	set(T &&f) noexcept : _set(forward<T>(f)) {}
	set &operator=(const set &) = delete;
	set &operator=(set &&) = delete;
	void operator=(Arg arg) const { _set(arg); }
};

template<class T>
set(T) -> set<function_argument_t<T, 0>>;

template<class R, class Arg>
class getset {
	function<R()> _get;
	function<void(Arg)> _set;
 public:
	template<class T1, class T2> requires (function_arity_v<T1> == 0
	  && function_arity_v<T2> == 1 && is_void_v<function_return_t<T2>>)
	getset(T1 &&f1, T2 &&f2) noexcept
		: _get(forward<T1>(f1)), _set(forward<T2>(f2)) {}
	getset &operator=(const getset &) = delete;
	getset &operator=(getset &&) = delete;
	operator R() const { return _get(); }
	void operator=(Arg arg) const { _set(arg); }
};

template<class T1, class T2>
getset(T1, T2) -> getset<function_return_t<T1>, function_argument_t<T2, 0>>;


// member function

template<class>
class member_function {};

#define _MEMBER_FUNCTION(cv, ...)                                        \
  template <typename R, typename C, typename... Args>                    \
  class member_function<R (C::*)(Args... __VA_ARGS__) cv> {              \
    R (C::*_member_function)(Args... __VA_ARGS__) cv;                    \
   public:                                                               \
    template<typename T>                                                 \
    member_function(T &&f) noexcept : _member_function(forward<T>(f)) {} \
	  R operator()(cv C &obj, Args... args __VA_ARGS__) const              \
	  { return (obj.*_member_function)(args...); }                         \
	  R operator()(cv C &&obj, Args... args __VA_ARGS__) const             \
	  { return (move(obj).*_member_function)(args...); }                   \
    R operator()(cv C *const obj, Args... args __VA_ARGS__) const        \
	  { return (obj->*_member_function)(args...); }                        \
  };                                                                     \
  template <typename R, typename C, typename... Args>                    \
  class member_function<R (C::*)(Args... __VA_ARGS__) cv &> {            \
    R (C::*_member_function)(Args... __VA_ARGS__) cv &;                  \
   public:                                                               \
    template<typename T>                                                 \
    member_function(T &&f) noexcept : _member_function(forward<T>(f)) {} \
  	R operator()(cv C &obj, Args... args __VA_ARGS__) const              \
  	{ return (obj.*_member_function)(args...); }                         \
    R operator()(cv C *const obj, Args... args __VA_ARGS__) const        \
	  { return (obj->*_member_function)(args...); }                        \
  };                                                                     \
  template <typename R, typename C, typename... Args>                    \
  class member_function<R (C::*)(Args... __VA_ARGS__) cv &&> {           \
    R (C::*_member_function)(Args... __VA_ARGS__) cv &&;                 \
   public:                                                               \
    template<typename T>                                                 \
    member_function(T &&f) noexcept : _member_function(forward<T>(f)) {} \
	  R operator()(cv C &&obj, Args... args __VA_ARGS__) const             \
	  { return (move(obj).*_member_function)(args...); }                   \
  };

_MEMBER_FUNCTION()
_MEMBER_FUNCTION(, , ...)
_MEMBER_FUNCTION(const)
_MEMBER_FUNCTION(const, , ...)
_MEMBER_FUNCTION(volatile)
_MEMBER_FUNCTION(volatile, , ...)
_MEMBER_FUNCTION(const volatile)
_MEMBER_FUNCTION(const volatile, , ...)

#undef _MEMBER_FUNCTION

template<typename T>
member_function(T) -> member_function<std::remove_cvref_t<T>>;


// promise

template<typename>
class promise;

template<typename R, typename... Args>
class promise<R(Args...)> {
	function<R(Args...)> _promise;
 public:
	template<typename T>
	promise(T &&f) noexcept: _promise(forward<T>(f)) {}
	R operator()(Args &&...args) const
	{ return _promise(forward<Args>(args)...); }
	template<typename T>
	promise<function_return_t<T>(Args...)> then(T &&f)  {
		return {[f, this](Args &&...args)
		{ return f(_promise(forward<Args>(args)...)); }};
	}
};

template<typename T>
promise(T) -> promise<function_traits_t<T>>;

}; // namespace xh

#endif //_XH_FUNCTION_TOOLS_H_
