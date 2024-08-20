// C++20 function.h
// Author: xupeigong@sjtu.edu.cn, 1583913466@qq.com
// Last Updated: 2024-08-20

// This header file defines a series of utilities for function wrapping,
// extending some of the capabilities not provided by the standard library. It
// includes implementations for getter and setter properties of classes,
// improved encapsulation of member functions, the addition of proxy functions
// for member functions, support for overloading-like function wrappers, the
// implementation of chainable function calls and pipe-like called function.

#ifndef _XH_FUNCTION_TOOLS_H_
#define _XH_FUNCTION_TOOLS_H_

#include <functional>
#include "function_traits.h"

using namespace std;

namespace xh {

/*********************
 * getter and setter *
 *********************/

template<class R>
class getter {
  function<R()> _getter;
 public:
  template<class T>
  getter(T &&f) noexcept : _getter(forward<T>(f)) {}
  operator R() const { return _getter(); }
};

template<class T>
getter(T) -> getter<function_return_t<T>>;

template<class Arg>
class setter {
  function<void(Arg)> _setter;
 public:
  template<class T>
  setter(T &&f) noexcept : _setter(forward<T>(f)) {}
  void operator=(Arg arg) const { _setter(arg); }
};

template<class T>
setter(T) -> setter<function_argument_t<T, 0>>;

template<class R, class Arg>
class getset {
  function<R()> _getter;
  function<void(Arg)> _setter;
 public:
  template<class T1, class T2>
  getset(T1 &&f1, T2 &&f2) noexcept
    : _getter(forward<T1>(f1)), _setter(forward<T2>(f2)) {}
  operator R() const { return _getter(); }
  void operator=(Arg arg) const { _setter(arg); }
};

template<class T1, class T2>
getset(T1, T2) -> getset<function_return_t<T1>, function_argument_t<T2, 0>>;


/*******************
 * member function *
 *******************/

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


/*************************
 * member function proxy *
 *************************/

#define MEMBER_FUNCTION_PROXY(member_function_pointer, signature, ...) {  \
    using class_type = std::remove_reference_t<                           \
      xh::function_class_t<decltype(member_function_pointer)>>;           \
    struct member_function_proxy {                                        \
      auto proxy signature {                                              \
        const auto proxy = reinterpret_cast<class_type *>(this);          \
        { __VA_ARGS__ }                                                   \
      }                                                                   \
    };                                                                    \
    auto proxy = &member_function_proxy::proxy;                           \
    member_function_pointer = reinterpret_cast<                           \
      xh::remove_member_function_class_t<decltype(proxy)> class_type::*>( \
      proxy);                                                             \
  };


/******************
 * multi function *
 ******************/

template<class... T>
struct multi_functor : T ... {
  using T::operator()...;
  constexpr multi_functor(T... f) noexcept: T(f)... {}
};

template<class... T>
class multi_function {
  tuple<T...> _multi_function;
  template<size_t N = 0, bool Strict = true, class... Args>
  auto call(Args &&... args) const {
    if constexpr (Strict) {
      if constexpr (N == sizeof...(T))
        return call<0, false>(forward<Args>(args)...);
      else if constexpr (is_same_v<function_argument_tuple<
          tuple_element_t<N, tuple<T...>>>, tuple<Args...>>)
        return get<N>(_multi_function)(forward<Args>(args)...);
      else return call<N + 1>(std::forward<Args>(args)...);
    } else {
      static_assert(N < sizeof...(T), "No matching function for call");
      if constexpr (is_invocable_v<tuple_element_t<N, tuple<T...>>, Args...>)
        return get<N>(_multi_function)(forward<Args>(args)...);
      else return call<N + 1, false>(std::forward<Args>(args)...);
    }
  }
 public:
  constexpr multi_function(T... funcs) noexcept: _multi_function(funcs...) {}
  template<class... Args>
  auto operator()(Args &&... args) const { return call(forward<Args>(args)...); }
};


/******************
 * function chain *
 ******************/

template<class>
class function_chain;

template<class R, class... Args>
class function_chain<R(Args...)> {
  function<R(Args...)> _function_chain;
 public:
  template<typename T>
  function_chain(T &&f) noexcept: _function_chain(forward<T>(f)) {}
  R operator()(Args &&...args) const { return _function_chain(forward<Args>(args)...); }
  template<typename T>
  function_chain<function_return_t<T>(Args...)> then(T &&f) {
    return {[g = forward<T>(f), this](Args &&...args) {
      return g(_function_chain(forward<Args>(args)...));
    }};
  }
};

template<typename T>
function_chain(T) -> function_chain<function_traits_t<T>>;


/*****************
 * pipe function *
 *****************/

template<class>
class function_pipe;

template<class R, class Arg0, class... Args>
class function_pipe<R(Arg0, Args...)> {
  function<R(Arg0, Args...)> _func;
  tuple<Args...> _args;
 public:
  template<class T>
  function_pipe(T &&f) : _func(forward<T>(f)) {}
  function_pipe<R(Arg0, Args...)> &operator()(Args... args) {
    _args = {args...};
    return *this;
  }
  friend R operator|(Arg0 arg0, const function_pipe<R(Arg0, Args...)> &pf)
  { return apply(pf._func, tuple_cat(tuple(arg0), pf._args)); }
};

template<class T>
function_pipe(T) -> function_pipe<function_traits_t<T>>;

}; // namespace xh

#endif //_XH_FUNCTION_TOOLS_H_
