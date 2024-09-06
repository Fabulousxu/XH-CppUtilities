// XH-CppUtilities
// C++20 function_utility.h
// Author: xupeigong@sjtu.edu.cn, 1583913466@qq.com
// Last Updated: 2024-09-05

#ifndef _XH_FUNCTION_UTILITY_H_
#define _XH_FUNCTION_UTILITY_H_

#include <functional>
#include <tuple>
#include <type_traits>

#include "function_traits.h"

namespace xh {

// getter and setter

template <class Ret>
class getter {
  std::function<Ret()> _getter;
 public:
  template <class T>
  getter(T &&f) noexcept : _getter(std::forward<T>(f)) {}
  operator Ret() const { return _getter(); }
};

template <class Arg>
class setter {
  std::function<void(Arg)> _setter;
 public:
  template <class T>
  setter(T &&f) noexcept : _setter(std::forward<T>(f)) {}
  void operator=(Arg arg) const { _setter(arg); }
};

template <class Ret, class Arg>
class getset {
  std::function<Ret()> _getter;
  std::function<void(Arg)> _setter;
 public:
  template <class Get, class Set>
  getset(Get &&g, Set &&s) noexcept
    : _getter(std::forward<Get>(g)), _setter(std::forward<Set>(s)) {}
  operator Ret() const { return _getter(); }
  void operator=(Arg arg) const { _setter(arg); }
};

template <class T>
getter(T) -> getter<function_return_t<T>>;

template <class T>
setter(T) -> setter<function_arg_t<T, 0>>;

template <class Get, class Set>
getset(Get, Set) -> getset<function_return_t<Get>, function_arg_t<Set, 0>>;

// member function proxy

#define MEMFUNC_PROXY(memfunc_ptr, signature, ...)                           \
  {                                                                          \
    using class_type =                                                       \
        std::remove_reference_t<xh::memfunc_class_t<decltype(memfunc_ptr)>>; \
    struct memfunc_proxy {                                                   \
      auto proxy signature {                                                 \
        const auto proxy = reinterpret_cast<class_type *>(this);             \
        __VA_ARGS__                                                          \
      }                                                                      \
    };                                                                       \
    auto proxy = &memfunc_proxy::proxy;                                      \
    memfunc_ptr = reinterpret_cast<                                          \
        xh::memfunc_function_t<decltype(proxy)> class_type::*>(proxy);       \
  };

// multi function

template <class... T>
struct multifunctor : T... {
  using T::operator()...;
  constexpr multifunctor(T... f) noexcept : T(f)... {}
};

template <class... T>
class multifunc {
  std::tuple<T...> _multifunc;
  template <size_t N = 0, bool Strict = true, class... Args>
  auto call(Args &&...args) const {
    if constexpr (Strict) {
      if constexpr (N == sizeof...(T))
        return call<0, false>(std::forward<Args>(args)...);
      else if constexpr (std::is_same_v<std::tuple<Args...>,
        function_arg_tuple<std::tuple_element_t<N, std::tuple<T...>>>>)
        return std::get<N>(_multifunc)(std::forward<Args>(args)...);
      else return call<N + 1>(std::forward<Args>(args)...);
    } else {
      static_assert(N < sizeof...(T), "No matching function for call");
      if constexpr (std::is_invocable_v<
        std::tuple_element_t<N, std::tuple<T...>>, Args...>)
        return std::get<N>(_multifunc)(std::forward<Args>(args)...);
      else return call<N + 1, false>(std::forward<Args>(args)...);
    }
  }
 public:
  constexpr multifunc(T... f) noexcept : _multifunc(f...) {}
  template <class... Args>
  auto operator()(Args &&...args) const {
    return call(std::forward<Args>(args)...);
  }
};

// function chain and pipe

template <class>
class funcchain;

template <class Ret, class... Args>
class funcchain<Ret(Args...)> {
  std::function<Ret(Args...)> _funcchain;
 public:
  template <class T>
  funcchain(T &&f) noexcept : _funcchain(std::forward<T>(f)) {}
  Ret operator()(Args... args) const { return _funcchain(args...); }
  template <class T>
  funcchain<function_return_t<T>(Args...)> then(T &&f) const {
    return {[f, this](Args... args) {
      if constexpr (std::is_void_v<Ret>) {
        _funcchain(args...);
        return f();
      } else return f(_funcchain(args...));
    }};
  }
};

template <class>
class funcpipe;

template <class Ret, class Arg0, class... Args>
class funcpipe<Ret(Arg0, Args...)> {
  std::function<Ret(Arg0, Args...)> _funcpipe;
  std::tuple<Args...> _args;
 public:
  template <class T>
  funcpipe(T &&f) : _funcpipe(std::forward<T>(f)) {}
  funcpipe<Ret(Arg0, Args...)> &operator()(Args... args) {
    _args = {args...};
    return *this;
  }
  friend Ret operator|(Arg0 arg0, const funcpipe<Ret(Arg0, Args...)> &pf) {
    return std::apply(pf._funcpipe, std::tuple_cat(std::tuple(arg0), pf._args));
  }
};

template <typename T>
funcchain(T) -> funcchain<function_traits_t<T>>;

template <class T>
funcpipe(T) -> funcpipe<function_traits_t<T>>;

}  // namespace xh

#endif  // !_XH_FUNCTION_UTILITY_H_
