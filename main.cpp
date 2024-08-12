#include <iostream>
#include "promise.h"
#include "enum_name.h"
#include "function.h"
//#include "cpp_helper.h"

using namespace xh;

template<class Ret>
class getter {
  std::function<Ret()> _getter;
 public:
  template<class Func>
  getter(Func &&f) : _getter(f) {}
  getter &operator=(const getter &) = delete;
  getter &operator=(getter &&) = delete;
  operator Ret() const { return _getter(); }
};
template<class Arg>
class setter {
  std::function<void(Arg)> _setter;
 public:
  setter(const std::function<void(Arg)> &f) : _setter(f) {}
  setter(std::function<void(Arg)> &&f) : _setter(std::move(f)) {}
  setter &operator=(const setter &) = delete;
  setter &operator=(setter &&) = delete;
  setter &operator=(Arg arg) const { _setter(arg); }
};

template<class T>
setter(T) -> setter<typename function_traits<T>::template argument_type<0>>;

#define get(name, return_type) \
const getter<return_type> name = [&]()->return_type
#define set(name, arg) \
const setter name = [&](arg)

struct A {
  static inline int k = 1;
  int count;

  get(getCntLessThanK, int) {
    return count - k;
  };

//  setter<int> setCntMoreThanK = [&](int val) {
//    count = val + k;
//  };

};

//============================main==========================

template<class T, class V=void>
struct is_callable:std::bool_constant<false>{};
template<class Func>
struct is_callable<Func, decltype(void(Func::operator()))>:std::bool_constant<true>{};
template<class Ret, class ...Args>
struct is_callable<Ret(Args...), void>:std::bool_constant<true>{};
template<class Ret, class ...Args>
struct is_callable<Ret(*)(Args...), void>:std::bool_constant<true>{};
template<class Ret, class ...Args>
struct is_callable<Ret(Args...,...), void>:std::bool_constant<true>{};
template<class Ret, class ...Args>
struct is_callable<Ret(*)(Args...,...), void>:std::bool_constant<true>{};


template<class T>
inline constexpr bool is_callable_v = is_callable<T>::value;

template<class T>
concept Callable = is_callable_v<T>;

int main(){
  const auto b = is_callable_v<int(int...)>;
  printf(b?"1":"0");  
  return 0;
}
