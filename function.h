#ifndef FUNCTION_TRAITS_H
#define FUNCTION_TRAITS_H

#include <tuple>

namespace xh {

template<typename T>
struct _remove_mem_fun_const_helper { using type = T; };
template<typename T>
struct _remove_mem_fun_volatile_helper { using type = T; };
template<typename T>
struct _remove_mem_fun_cv_helper { using type = T; };

#define REMOVE_MEM_FUN_CV_HELPER(cv, c_res, v_res, cv_res)                    \
template<typename Ret, typename Class, typename... Args>                      \
struct _remove_mem_fun_##cv##_helper<Ret(Class::*)(Args...)> {                \
  using type = Ret(Class::*)(Args...);                                        \
};                                                                            \
template<typename Ret, typename Class, typename... Args>                      \
struct _remove_mem_fun_##cv##_helper<Ret(Class::*)(Args...) const> {          \
  using type = Ret(Class::*)(Args...) c_res;                                  \
};                                                                            \
template<typename Ret, typename Class, typename... Args>                      \
struct _remove_mem_fun_##cv##_helper<Ret(Class::*)(Args...) volatile> {       \
  using type = Ret(Class::*)(Args...) v_res;                                  \
};                                                                            \
template<typename Ret, typename Class, typename... Args>                      \
struct _remove_mem_fun_##cv##_helper<Ret(Class::*)(Args...) const volatile> { \
  using type = Ret(Class::*)(Args...) cv_res;                                 \
};

REMOVE_MEM_FUN_CV_HELPER(const, , , volatile)
REMOVE_MEM_FUN_CV_HELPER(volatile, const, , const)
REMOVE_MEM_FUN_CV_HELPER(cv, , ,)
#undef REMOVE_MEM_FUN_CV_HELPER

template<typename T>
struct remove_mem_fun_const : _remove_mem_fun_const_helper<T> {};
template<typename T>
struct remove_mem_fun_volatile : _remove_mem_fun_volatile_helper<T> {};
template<typename T>
struct remove_mem_fun_cv : _remove_mem_fun_cv_helper<T> {};
template<typename T>
using remove_mem_fun_const_t = remove_mem_fun_const<T>::type;
template<typename T>
using remove_mem_fun_volatile_t = remove_mem_fun_volatile<T>::type;
template<typename T>
using remove_mem_fun_cv_t = remove_mem_fun_cv<T>::type;

template<typename Functor>
struct _function_traits_helper : _function_traits_helper<
    remove_mem_fun_cv_t<decltype(&Functor::operator())>> {
};
template<typename Ret, typename... Args>
struct _function_traits_helper<Ret(Args...)> {
  using type = Ret(Args...);
  using return_type = Ret;
  using argument_types = std::tuple<Args...>;
  template<unsigned index>
  using argument_type = std::tuple_element_t<index, std::tuple<Args...>>;
  inline static constexpr unsigned arity = sizeof...(Args);
};
template<typename Ret, typename... Args>
struct _function_traits_helper<Ret(*)(Args...)>
    : _function_traits_helper<Ret(Args...)> {
};
template<typename Ret, typename Class, typename... Args>
struct _function_traits_helper<Ret(Class::*)(Args...)>
    : _function_traits_helper<Ret(Args...)> {
};

template<typename T>
struct function_traits : _function_traits_helper<
    remove_mem_fun_cv_t<std::remove_cvref_t<T>>> {
};
template<typename T>
using function_traits_t = function_traits<T>::type;
template<typename T>
using function_return_t = function_traits<T>::return_type;

template<typename>
class mem_fun;

#define MEM_FUN(cv)                                              \
template<typename Ret, typename Class, typename... Args>         \
class mem_fun<Ret(Class::*)(Args...) cv> {                       \
  Ret (Class::*_mem_fun)(Args...) cv;                            \
 public:                                                         \
  mem_fun(Ret (Class::*f)(Args...) cv) noexcept : _mem_fun(f) {} \
  Ret operator()(cv Class &obj, Args... args) const {            \
    return (obj.*_mem_fun)(args...);                             \
  }                                                              \
  Ret operator()(cv Class *obj, Args... args) const {            \
    return (obj->*_mem_fun)(args...);                            \
  }                                                              \
};

MEM_FUN()
MEM_FUN(const)
MEM_FUN(volatile)
MEM_FUN(const volatile)
#undef MEM_FUN

template<typename T>
mem_fun(T) -> mem_fun<std::remove_cvref_t<T>>;

}; // namespace xh

#endif //FUNCTION_TRAITS_H
