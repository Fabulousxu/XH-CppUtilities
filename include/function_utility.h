// XH-CppUtilities
// C++20 function_utility.h
// Author: xupeigong@sjtu.edu.cn, 1583913466@qq.com
// Last Updated: 2024-09-12

#ifndef _XH_FUNCTION_UTILITY_H_
#define _XH_FUNCTION_UTILITY_H_

#include <functional>
#include <tuple>
#include <type_traits>

#include "function_traits.h"

namespace xh {

using namespace std;

// function

template <class>
class function;

template <class MFP> requires is_memfunc_v<MFP>
class function<MFP> {
 public:
  function(MFP mfp) noexcept : _mfp(mfp) {}
  function() noexcept = default;
  function(const function &) noexcept = default;
  function(function &&) noexcept = default;
  ~function() noexcept = default;
  function &operator=(const function &) noexcept = default;
  function &operator=(function &&) noexcept = default;
  function &operator=(MFP mfp) noexcept { _mfp = mfp; return *this; }
  operator bool() const noexcept { return _mfp; }

  template <class Arg0, class... Args>
    requires is_invocable_v<MFP, Arg0, Args...>
  funcret_t<MFP> operator()(Arg0 &&arg0, Args &&...args) const
    noexcept(funcqual_of_v<mffunction_t<MFP>> & funcqual_mask::noexcept_mask) {
    if constexpr (is_pointer_v<decay_t<Arg0>>)
      return (forward<Arg0>(arg0)->*_mfp)(forward<Args>(args)...);
    else return (forward<Arg0>(arg0).*_mfp)(forward<Args>(args)...);
  }
 private:
  MFP _mfp = nullptr;
};

template <class Ret, class... Args>
class function<Ret(Args...)> {
 public:
  template <class T> requires (!is_same_v<decay_t<T>, function>)
  function(T &&f) noexcept : fbp(new funcimpl<decay_t<T>>(forward<T>(f))) {}

  function() noexcept = default;
  function(const function &r) noexcept : fbp(r.fbp ? r.fbp->copy() : nullptr) {}
  function(function &&r) noexcept : fbp(r.fbp) { r.fbp = nullptr; }
  ~function() noexcept { delete fbp; }
  
  function &operator=(const function &r) noexcept {
    if (this != &r) {
      delete fbp;
      fbp = r.fbp ? r.fbp->copy() : nullptr;
    }
    return *this;
  }
  
  function &operator=(function &&r) noexcept {
    swap(fbp, r.fbp);
    return *this;
  }

  template <class T> requires (!is_same_v<decay_t<T>, function>)
  function &operator=(T &&f) noexcept {
    delete fbp;
    fbp = new funcimpl<decay_t<T>>(forward<T>(f));
    return *this;
  }

  operator bool() const noexcept { return fbp; }
  
  Ret operator()(Args... args) const {
    return fbp->call(forward<Args>(args)...);
  }

 private:
  struct funcbase {
    virtual Ret call(Args...) const = 0;
    virtual funcbase *copy() const noexcept = 0;
    virtual ~funcbase() noexcept = default;
  } *fbp = nullptr;

  template <class T> requires (is_funcptr_v<T> || is_functor_v<T>)
  struct funcimpl : funcbase {
    T f;
    template <class F> requires is_same_v<decay_t<F>, T>
    funcimpl(F &&f) : f(forward<F>(f)) {}

    Ret call(Args... args) const override { return f(forward<Args>(args)...); }
    funcimpl *copy() const noexcept override { return new funcimpl(f); }
    ~funcimpl() noexcept override = default;
  };
};

template <class T>
function(T) -> function<functraits_t<T>>;

// multi function

template <class... T>
struct multifunctor : T... {
  using T::operator()...;
  constexpr multifunctor(T... f) noexcept : T(forward<T>(f))... {}
};

template <class... T>
class multifunc {
 public:
  template <class... F>
    requires (sizeof...(F) > 1 || (sizeof...(F) == 1 &&
      !is_same_v<decay_t<nth_of_t<0, F...>>, multifunc>))
  multifunc(F &&...f) noexcept : ftuple(forward<F>(f)...) {}
  
  multifunc() noexcept = default;
  multifunc(const multifunc &) noexcept = default;
  multifunc(multifunc &&) noexcept = default;
  ~multifunc() noexcept = default;
  multifunc &operator=(const multifunc &) noexcept = default;
  multifunc &operator=(multifunc &&) noexcept = default;

  template <class... F>
    requires (sizeof...(F) > 1 || (sizeof...(F) == 1 &&
      !is_same_v<decay_t<nth_of_t<0, F...>>, multifunc>))
  multifunc &operator=(F &&...f) {
    ftuple = {forward<F>(f)...};
    return *this;
  }

 private:
  template <size_t N, bool Strict, class... Args>
  static constexpr bool match() {
    if constexpr (Strict)
      if constexpr (N == sizeof...(T)) return match<0, false, Args...>();
      else if constexpr (is_same_v<tuple<Args...>, funcarg_tuple<fn_t<N>>>)
        return true;
      else return match<N + 1, true, Args...>();
    else if (N >= sizeof...(T)) return false;
    else if constexpr (is_invocable_v<fn_t<N>, Args...>) return true;
    else return match<N + 1, false, Args...>();
  }

 public:
  template <class... Args>
  static constexpr bool is_match_v = match<0, true, Args...>();

  template <class... Args> requires is_match_v<Args...>
  decltype(auto) operator()(Args &&...args) const {
    return call<0, true>(forward<Args>(args)...);
  }

 private:
  tuple<std::function<T>...> ftuple;

  template <size_t N>
  using fn_t = nth_of_t<N, T...>;

  template <size_t N, bool Strict, class... Args>
  decltype(auto) call(Args &&...args) const {
    if constexpr (Strict)
      if constexpr (N >= sizeof...(T))
        return call<0, false>(forward<Args>(args)...);
      else if constexpr (is_same_v<tuple<Args...>, funcarg_tuple<fn_t<N>>>)
        return get<N>(ftuple)(forward<Args>(args)...);
      else return call<N + 1, true>(forward<Args>(args)...);
    else {
      static_assert(N < sizeof...(T), "No matching function for call");
      if constexpr (is_invocable_v<fn_t<N>, Args...>)
        return get<N>(ftuple)(forward<Args>(args)...);
      else return call<N + 1, false>(forward<Args>(args)...);
    }
  }
};

template <class... T>
multifunc(T...) -> multifunc<functraits_t<T>...>;

// function chain and pipe

template <class>
class funcchain;

template <class Ret, class... Args>
class funcchain<Ret(Args...)> {
 public:
  template <class T> requires (!is_same_v<decay_t<T>, funcchain>)
  funcchain(T &&f) : fc(forward<T>(f)) {}

  funcchain() noexcept = default;
  funcchain(const funcchain &rhs) noexcept = default;
  funcchain(funcchain &&rhs) noexcept = default;
  ~funcchain() noexcept = default;
  funcchain &operator=(const funcchain &) noexcept = default;
  funcchain &operator=(funcchain &&) noexcept = default;

  template <class T> requires (!is_same_v<decay_t<T>, funcchain>)
  funcchain &operator=(T &&f) {
    fc = forward<T>(f);
    return *this;
  }
  
  operator bool() const noexcept { return fc; }
  Ret operator()(Args... args) const { return fc(forward<Args>(args)...); }

  template <class T> requires (is_invocable_v<T, Ret> || (funcarity_v<T> == 0))
  funcchain<funcret_t<T>(Args...)> then(T &&f) const {
    return {[f = move(f), fc = this->fc](Args... args) {
      if constexpr (is_void_v<Ret>) {
        fc(forward<Args>(args)...);
        return f();
      } else return f(fc(forward<Args>(args)...));
    }};
  }
 private:
  std::function<Ret(Args...)> fc;
};

template <class T>
funcchain(T) -> funcchain<functraits_t<T>>;

template <class>
class funcpipe;

template <class Ret, class Arg0, class... Args>
class funcpipe<Ret(Arg0, Args...)> {
 public:
  template <class T> requires (!is_same_v<decay_t<T>, funcpipe>)
  funcpipe(T &&f) : fp(forward<T>(f)) {}

  funcpipe() noexcept = default;
  funcpipe(const funcpipe &) noexcept = default;
  funcpipe(funcpipe &&) noexcept = default;
  ~funcpipe() noexcept = default;
  funcpipe &operator=(const funcpipe &) noexcept = default;
  funcpipe &operator=(funcpipe &&) noexcept = default;

  template <class T> requires (!is_same_v<decay_t<T>, funcpipe>)
  funcpipe &operator=(T &&f) {
    fp = forward<T>(f);
    return *this;
  }

  funcpipe &operator()(Args... args) {
    argtuple = {forward<Args>(args)...};
    return *this;
  }

  friend Ret operator|(Arg0 arg0, const funcpipe &fp) {
    return apply(fp.fp, tuple_cat(tuple(arg0), fp.argtuple));
  }

 private:
  std::function<Ret(Arg0, Args...)> fp;
  tuple<Args...> argtuple;
};

template <class T>
funcpipe(T) -> funcpipe<functraits_t<T>>;

// getter and setter

template <class Ret>
class getter {
 public:
  template <class T> requires (!is_same_v<decay_t<T>, getter>)
  getter(T &&f) : g(forward<T>(f)) {}

  getter() = delete;
  getter(const getter &) = delete;
  getter(getter &&) = delete;
  ~getter() noexcept = default;
  getter &operator=(const getter &) = delete;
  getter &operator=(getter &&) = delete;
  operator Ret() const { return g(); }
 private:
  std::function<Ret()> g;
};

template <class T>
getter(T) -> getter<funcret_t<T>>;

template <class Arg>
class setter {
 public:
  template <class T> requires (!is_same_v<decay_t<T>, setter>)
  setter(T &&s) : s(forward<T>(s)) {}

  setter() = delete;
  setter(const setter &) = delete;
  setter(setter &&) = delete;
  ~setter() noexcept = default;
  setter &operator=(const setter &) = delete;
  setter &operator=(setter &&) = delete;
  void operator=(Arg arg) { s(forward<Arg>(arg)); }
 private:
  std::function<void(Arg)> s;
};

template <class T>
setter(T) -> setter<funcarg_t<T, 0>>;

template <class Ret, class Arg>
class getset {
 public:
  template <class G, class S>
  getset(G &&g, S &&s) : g(forward<G>(g)), s(forward<S>(s)) {}

  getset() = delete;
  getset(const getset &) = delete;
  getset(getset &&) = delete;
  ~getset() noexcept = default;
  getset &operator=(const getset &) = delete;
  getset &operator=(getset &&) = delete;
  operator Ret() const { return g(); }
  void operator=(Arg arg) { s(forward<Arg>(arg)); }
 private:
  std::function<Ret()> g;
  std::function<void(Arg)> s;
};

template <class G, class S>
getset(G, S) -> getset<funcret_t<G>, funcarg_t<S, 0>>;

// auto return

struct auto_return_t {
  template <class T>
  operator T() const { return T{}; }
};

#define auto_return return xh::auto_return_t{};

// member function proxy

#define MEMFUNC_PROXY(mfp, signature, ...)                       \
  {                                                              \
    using class_type = xh::mfclass_t<decltype(mfp)>;             \
    struct memfunc_proxy {                                       \
      auto proxy signature {                                     \
        const auto proxy = reinterpret_cast<class_type *>(this); \
        __VA_ARGS__                                              \
      }                                                          \
    };                                                           \
    auto proxy = &memfunc_proxy::proxy;                          \
    memfunc_ptr = reinterpret_cast<                              \
      xh::mffunction_t<decltype(proxy)> class_type::*>(proxy);   \
  };

} // namespace xh

#endif // !_XH_FUNCTION_UTILITY_H_
