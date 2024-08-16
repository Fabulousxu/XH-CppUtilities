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

#define auto_return return xh::_auto_return{};


// getter and setter

template<class R>
class getter {
	function<R()> _getter;
 public:
	template<class T> requires (function_arity_v<T> == 0)
	getter(T &&f) noexcept : _getter(forward<T>(f)) {}
	getter &operator=(const getter &) = delete;
	getter &operator=(getter &&) = delete;
	operator R() const { return _getter(); }
};

template<class T>
getter(T) -> getter<function_return_t<T>>;

template<class Arg>
class setter {
	function<void(Arg)> _setter;
 public:
	template<class T> requires (function_arity_v<T> == 1
	  && is_void_v<function_return_t<T>>)
	setter(T &&f) noexcept : _setter(forward<T>(f)) {}
	setter &operator=(const setter &) = delete;
	setter &operator=(setter &&) = delete;
	setter &operator=(Arg arg) const { _setter(arg); }
};

template<class T>
setter(T) -> setter<function_argument_t<T, 0>>;

template<class R, class Arg>
class getset {
	function<R()> _getter;
	function<void(Arg)> _setter;
 public:
	template<class T1, class T2> requires (function_arity_v<T1> == 0
	  && function_arity_v<T2> == 1 && is_void_v<function_return_t<T2>>)
	getset(T1 &&f1, T2 &&f2) noexcept
		: _getter(forward<T1>(f1)), _setter(forward<T2>(f2)) {}
	getset &operator=(const getset &) = delete;
	getset &operator=(getset &&) = delete;
	operator R() const { return _getter(); }
	void operator=(Arg arg) const { _setter(arg); }
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
member_function(T) -> member_function<remove_cvref_t<T>>;


// member function proxy

#define MEMBER_FUNCTION_PROXY(member_function_pointer, signature, ...) {       \
    using class_type =                                                         \
		  remove_reference_t<function_class_t<decltype(member_function_pointer)>>; \
    struct member_function_proxy {                                             \
      auto proxy signature {                                                   \
        const auto proxy = reinterpret_cast<class_type *>(this);               \
        { __VA_ARGS__ }                                                        \
      }                                                                        \
    };                                                                         \
    auto proxy = &member_function_proxy::proxy;                                \
    member_function_pointer = reinterpret_cast<                                \
        remove_member_function_class_t<decltype(proxy)> class_type::*>(proxy); \
  };


// multi function

template<class... T>
struct multi_functor : public T... {
	constexpr multi_functor(T... f) noexcept : T(f)... {}
	using T::operator()...;
};

template<class... T>
class multi_function {
	tuple<T...> _multi_function;
	template<size_t N, class... Args>
	auto call(Args &&... args) const {
		static_assert(N < sizeof...(T), "No matching function for call");
		if constexpr (is_invocable_v<tuple_element_t<N, tuple<T...>>, Args...>)
			return get<N>(_multi_function)(forward<Args>(args)...);
		else return call<N + 1>(std::forward<Args>(args)...);
	}
 public:
	constexpr multi_function(T... funcs) noexcept: _multi_function(funcs...) {}
	template<class... Args>
	auto operator()(Args &&... args) const
	{ return call<0>(forward<Args>(args)...); }
};


// function_chain

template<typename>
class function_chain;

template<typename R, typename... Args>
class function_chain<R(Args...)> {
	function<R(Args...)> _function_chain;
 public:
	template<typename T>
	function_chain(T &&f) noexcept: _function_chain(forward<T>(f)) {}
	R operator()(Args &&...args) const
	{ return _function_chain(forward<Args>(args)...); }
	template<typename T>
	function_chain<function_return_t<T>(Args...)> then(T &&f)  {
		return {[g = forward<T>(f), this](Args &&...args)
		{ return g(_function_chain(forward<Args>(args)...)); }};
	}
};

template<typename T>
function_chain(T) -> function_chain<function_traits_t<T>>;

}; // namespace xh

#endif //_XH_FUNCTION_TOOLS_H_
