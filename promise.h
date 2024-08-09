#ifndef PROMISE_H
#define PROMISE_H

#include <functional>
#include "function.h"

namespace xh {

template<typename T>
class Promise;

template<typename Ret, typename... Args>
class Promise<Ret(Args...)> {
 public:
  Promise(const std::function<Ret(Args...)> &f) noexcept: _fun(f) {}
  Promise(std::function<Ret(Args...)> &&f) noexcept: _fun(std::move(f)) {}
  Ret operator()(Args &&...args) { return _fun(std::forward<Args>(args)...); }
  template<typename Func>
  Promise<std::invoke_result_t<Func, Ret>(Args...)> then(Func &&f) {
    return {[f, this](Args &&...args) {
      return f(_fun(std::forward<Args>(args)...));
    }};
  }
 private:
  std::function<Ret(Args...)> _fun;
};

template<typename T>
Promise(T) -> Promise<function_traits_t<T>>;

}; // namespace xh

#endif //PROMISE_H
