#ifndef PROMISE_H
#define PROMISE_H

#include "function_traits.h"

template<typename T>
class Promise;

template<typename Ret, typename... Args>
class Promise<Ret(Args...)> {
 public:
  Promise(const std::function<Ret(Args...)> &f) noexcept: m_f(f) {}
  Promise(std::function<Ret(Args...)> &&f) noexcept: m_f(std::move(f)) {}

  Ret operator()(Args &&...args) { return m_f(std::forward<Args>(args)...); }

  template<typename Func>
  Promise<std::invoke_result_t<Func, Ret>(Args...)> then(Func &&f) {
    return {[f, this](Args &&...args) {
      return f(m_f(std::forward<Args>(args)...));
    }};
  }
 private:
  std::function<Ret(Args...)> m_f;
};

template<typename T>
Promise(T) -> Promise<function_traits_t<T>>;

#endif //PROMISE_H
