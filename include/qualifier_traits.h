// XH-CppUtilities
// C++20 qualifier_traits.h
// Author: xupeigong@sjtu.edu.cn
// Last Updated: 2024-09-05

#ifndef _XH_QUALIFIER_TRAITS_H_
#define _XH_QUALIFIER_TRAITS_H_

#include <type_traits>

namespace xh {

// qualifier

struct qualifier {
  enum class id {
             value,
       const_value,
    volatile_value,
          cv_value,
              lref,
        const_lref,
     volatile_lref,
           cv_lref,
              rref,
        const_rref,
     volatile_rref,
           cv_rref,
  };

  using enum id;

  static constexpr bool is_const(id q) {
    switch (q) {
      case    const_value:
      case       cv_value: return true;
      default:             return false;
    }
  }

  static constexpr bool is_volatile(id q) {
    switch (q) {
      case volatile_value:
      case       cv_value: return true;
      default:             return false;
    }
  }

  static constexpr bool is_lref(id q) {
    switch (q) {
      case           lref:
      case     const_lref:
      case  volatile_lref:
      case        cv_lref: return true;
      default:             return false;
    }
  }

  static constexpr bool is_rref(id q) {
    switch (q) {
      case           rref:
      case     const_rref:
      case  volatile_rref:
      case        cv_rref: return true;
      default:             return false;
    }
  }

  static constexpr bool is_ref(id q) {
    switch (q) {
      case           lref:
      case     const_lref:
      case  volatile_lref:
      case        cv_lref:
      case           rref:
      case     const_rref:
      case  volatile_rref:
      case        cv_rref: return true;
      default:             return false;
    }
  }

  static constexpr id add_const(id q) {
    switch (q) {
      case          value: return    const_value;
      case volatile_value: return       cv_value;
      default:             return              q;
    }
  }

  static constexpr id add_volatile(id q) {
    switch (q) {
      case          value: return volatile_value;
      case    const_value: return       cv_value;
      default:             return              q;
    }
  }

  static constexpr id add_cv(id q) {
    switch (q) {
      case          value:
      case    const_value:
      case volatile_value: return       cv_value;
      default:             return              q;
    }
  }

  static constexpr id add_lref(id q) {
    switch (q) {
      case          value:
      case           rref: return           lref;
      case     const_rref:
      case    const_value: return     const_lref;
      case  volatile_rref:
      case volatile_value: return  volatile_lref;
      case        cv_rref:
      case       cv_value: return        cv_lref;
      default:             return              q;
    }
  }

  static constexpr id add_rref(id q) {
    switch (q) {
      case          value: return           rref;
      case    const_value: return     const_rref;
      case volatile_value: return  volatile_rref;
      case       cv_value: return        cv_rref;
      default:             return              q;
    }
  }

  static constexpr id add_qualifier(id q1, id q2) {
    switch (q2) {
      case          value: return q1;
      case    const_value: return add_const(q1);
      case volatile_value: return add_volatile(q1);
      case       cv_value: return add_cv(q1);
      case           lref: return add_lref(q1);
      case     const_lref: return add_lref(add_const(q1));
      case  volatile_lref: return add_lref(add_volatile(q1));
      case        cv_lref: return add_lref(add_cv(q1));
      case           rref: return add_rref(q1);
      case     const_rref: return add_rref(add_const(q1));
      case  volatile_rref: return add_rref(add_volatile(q1));
      case        cv_rref: return add_rref(add_cv(q1));
    }
  }

  static constexpr id remove_const(id q) {
    switch (q) {
      case    const_value: return          value;
      case       cv_value: return volatile_value;
      default:             return              q;
    }
  }

  static constexpr id remove_volatile(id q) {
    switch (q) {
      case volatile_value: return          value;
      case       cv_value: return    const_value;
      default:             return              q;
    }
  }

  static constexpr id remove_cv(id q) {
    switch (q) {
      case    const_value:
      case volatile_value:
      case       cv_value: return          value;
      default:             return              q;
    }
  }

  static constexpr id remove_ref(id q) {
    switch (q) {
      case           lref:
      case           rref: return          value;
      case     const_lref:
      case     const_rref: return    const_value;
      case  volatile_lref:
      case  volatile_rref: return volatile_value;
      case        cv_lref:
      case        cv_rref: return       cv_value;
      default:             return              q;
    }
  }

  static constexpr id remove_cvref(id q) { return value; }
};

using qualifier_id = qualifier::id;


// qualifier traits

using enum qualifier_id;

template <class T, class D, qualifier_id Q>
struct _qualifier_traits_helper
  : std::integral_constant<qualifier_id, Q> {
  using       type = T;
  using decay_type = D;
};

template <class T>
struct qualifier_traits
  : _qualifier_traits_helper<T, T, value> {};

template <class T>
struct qualifier_traits<const T>
  : _qualifier_traits_helper<const T, T, const_value> {};

template <class T>
struct qualifier_traits<volatile T>
  : _qualifier_traits_helper<volatile T, T, volatile_value> {};

template <class T>
struct qualifier_traits<const volatile T>
  : _qualifier_traits_helper<const volatile T, T, cv_value> {};

template <class T>
struct qualifier_traits<T &>
  : _qualifier_traits_helper<T &, T, lref> {};

template <class T>
struct qualifier_traits<const T &>
  : _qualifier_traits_helper<const T &, T, const_lref> {};

template <class T>
struct qualifier_traits<volatile T &>
  : _qualifier_traits_helper<volatile T &, T, volatile_lref> {};

template <class T>
struct qualifier_traits<const volatile T &>
  : _qualifier_traits_helper<const volatile T &, T, cv_lref> {};

template <class T>
struct qualifier_traits<T &&>
  : _qualifier_traits_helper<T &&, T, rref> {};

template <class T>
struct qualifier_traits<const T &&>
  : _qualifier_traits_helper<const T &&, T, const_rref> {};

template <class T>
struct qualifier_traits<volatile T &&>
  : _qualifier_traits_helper<volatile T &&, T, volatile_rref> {};

template <class T>
struct qualifier_traits<const volatile T &&>
  : _qualifier_traits_helper<const volatile T &&, T, cv_rref> {};


template <class>
struct funcqual_traits {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args...)>
  : _qualifier_traits_helper<Ret(Args...),
      Ret(Args...), value> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args..., ...)>
  : _qualifier_traits_helper<Ret(Args..., ...),
      Ret(Args..., ...), value> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args...) const>
  : _qualifier_traits_helper<Ret(Args...) const,
      Ret(Args...), const_value> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args..., ...) const>
  : _qualifier_traits_helper<Ret(Args..., ...) const,
      Ret(Args..., ...), const_value> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args...) volatile>
  : _qualifier_traits_helper<Ret(Args...) volatile,
      Ret(Args...), volatile_value> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args..., ...) volatile>
  : _qualifier_traits_helper<Ret(Args..., ...) volatile,
      Ret(Args..., ...), volatile_value> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args...) const volatile>
  : _qualifier_traits_helper<Ret(Args...) const volatile,
      Ret(Args...), cv_value> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args..., ...) const volatile>
  : _qualifier_traits_helper<Ret(Args..., ...) const volatile,
      Ret(Args..., ...), cv_value> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args...) &>
  : _qualifier_traits_helper<Ret(Args...) &,
      Ret(Args...), lref> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args..., ...) &>
  : _qualifier_traits_helper<Ret(Args..., ...) &,
      Ret(Args..., ...), lref> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args...) const &>
  : _qualifier_traits_helper<Ret(Args...) const &,
      Ret(Args...), const_lref> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args..., ...) const &>
  : _qualifier_traits_helper<Ret(Args..., ...) const &,
      Ret(Args..., ...), const_lref> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args...) volatile &>
  : _qualifier_traits_helper<Ret(Args...) volatile &,
      Ret(Args...), volatile_lref> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args..., ...) volatile &>
  : _qualifier_traits_helper<Ret(Args..., ...) volatile &,
      Ret(Args..., ...), volatile_lref> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args...) const volatile &>
  : _qualifier_traits_helper<Ret(Args...) const volatile &,
      Ret(Args...), cv_lref> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args..., ...) const volatile &>
  : _qualifier_traits_helper<Ret(Args..., ...) const volatile &,
      Ret(Args..., ...), cv_lref> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args...) &&>
  : _qualifier_traits_helper<Ret(Args...) &&,
      Ret(Args...), rref> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args..., ...) &&>
  : _qualifier_traits_helper<Ret(Args..., ...) &&,
      Ret(Args..., ...), rref> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args...) const &&>
  : _qualifier_traits_helper<Ret(Args...) const &&,
      Ret(Args...), const_rref> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args..., ...) const &&>
  : _qualifier_traits_helper<Ret(Args..., ...) const &&,
      Ret(Args..., ...), const_rref> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args...) volatile &&>
  : _qualifier_traits_helper<Ret(Args...) volatile &&,
      Ret(Args...), volatile_rref> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args..., ...) volatile &&>
  : _qualifier_traits_helper<Ret(Args..., ...) volatile &&,
      Ret(Args..., ...), volatile_rref> {};

template <class Ret, class... Args>
struct funcqual_traits<Ret(Args...) const volatile &&>
  : _qualifier_traits_helper<Ret(Args...) const volatile &&,
      Ret(Args...), cv_rref> {};


template <class T>
inline constexpr qualifier_id qualifier_traits_v = qualifier_traits<T>::value;

template <class T>
using qualifier_decay_t = qualifier_traits<T>::decay_type;

template <class T>
inline constexpr qualifier_id funcqual_traits_v = funcqual_traits<T>::value;

template <class T>
using funcqual_decay_t = funcqual_traits<T>::decay_type;


// qualifier check

template <class T>
using is_const = std::is_const<T>;

template <class T>
using is_volatile = std::is_volatile<T>;

template <class T>
using is_lref = std::is_lvalue_reference<T>;

template <class T>
using is_rref = std::is_rvalue_reference<T>;

template <class T>
using is_ref = std::is_reference<T>;

template <class T>
inline constexpr bool is_const_v = std::is_const_v<T>;

template <class T>
inline constexpr bool is_volatile_v = std::is_volatile_v<T>;

template <class T>
inline constexpr bool is_lref_v = std::is_lvalue_reference_v<T>;

template <class T>
inline constexpr bool is_rref_v = std::is_rvalue_reference_v<T>;

template <class T>
inline constexpr bool is_ref_v = std::is_reference_v<T>;


template <class T>
inline constexpr bool function_is_const_v =
  qualifier::is_const(funcqual_traits_v<T>);

template <class T>
inline constexpr bool function_is_volatile_v =
  qualifier::is_volatile(funcqual_traits_v<T>);

template <class T>
inline constexpr bool function_is_lref_v =
  qualifier::is_lref(funcqual_traits_v<T>);

template <class T>
inline constexpr bool function_is_rref_v =
  qualifier::is_rref(funcqual_traits_v<T>);

template <class T>
inline constexpr bool function_is_ref_v =
  qualifier::is_ref(funcqual_traits_v<T>);

template <class T>
using function_is_const = std::bool_constant<function_is_const_v<T>>;

template <class T>
using function_is_volatile = std::bool_constant<function_is_volatile_v<T>>;

template <class T>
using function_is_lref = std::bool_constant<function_is_lref_v<T>>;

template <class T>
using function_is_rref = std::bool_constant<function_is_rref_v<T>>;

template <class T>
using function_is_ref = std::bool_constant<function_is_ref_v<T>>;


// add qualifier

template <class T>
using add_const = std::add_const<T>;

template <class T>
using add_volatile = std::add_volatile<T>;

template <class T>
using add_cv = std::add_cv<T>;

template <class T>
using add_lref = std::add_lvalue_reference<T>;

template <class T>
using add_rref = std::add_rvalue_reference<T>;

template <class T>
using add_const_t = std::add_const_t<T>;

template <class T>
using add_volatile_t = std::add_volatile_t<T>;

template <class T>
using add_cv_t = std::add_cv_t<T>;

template <class T>
using add_lref_t = std::add_lvalue_reference_t<T>;

template <class T>
using add_rref_t = std::add_rvalue_reference_t<T>;

template <class, qualifier_id>
struct add_qualifier {};

template <class T>
struct add_qualifier<T, value> : std::type_identity<T> {};

template <class T>
struct add_qualifier<T, const_value> : add_const<T> {};

template <class T>
struct add_qualifier<T, volatile_value> : add_volatile<T> {};

template <class T>
struct add_qualifier<T, cv_value> : add_cv<T> {};

template <class T>
struct add_qualifier<T, lref> : add_lref<T> {};

template <class T>
struct add_qualifier<T, const_lref> : add_lref<add_const_t<T>> {};

template <class T>
struct add_qualifier<T, volatile_lref> : add_lref<add_volatile_t<T>> {};

template <class T>
struct add_qualifier<T, cv_lref> : add_lref<add_cv_t<T>> {};

template <class T>
struct add_qualifier<T, rref> : add_rref<T> {};

template <class T>
struct add_qualifier<T, const_rref> : add_rref<add_const_t<T>> {};

template <class T>
struct add_qualifier<T, volatile_rref> : add_rref<add_volatile_t<T>> {};

template <class T>
struct add_qualifier<T, cv_rref> : add_rref<add_cv_t<T>> {};


template <class T, qualifier_id Q>
struct add_funcqual
  : add_funcqual<funcqual_decay_t<T>,
      qualifier::add_qualifier(funcqual_traits_v<T>, Q)> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args...), value>
  : std::type_identity<Ret(Args...)> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args..., ...), value>
  : std::type_identity<Ret(Args..., ...)> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args...), const_value>
  : std::type_identity<Ret(Args...) const> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args..., ...), const_value>
  : std::type_identity<Ret(Args..., ...) const> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args...), volatile_value>
  : std::type_identity<Ret(Args...) volatile> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args..., ...), volatile_value>
  : std::type_identity<Ret(Args..., ...) volatile> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args...), cv_value>
  : std::type_identity<Ret(Args...) const volatile> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args..., ...), cv_value>
  : std::type_identity<Ret(Args..., ...) const volatile> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args...), lref>
  : std::type_identity<Ret(Args...) &> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args..., ...), lref>
  : std::type_identity<Ret(Args..., ...) &> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args...), const_lref>
  : std::type_identity<Ret(Args...) const &> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args..., ...), const_lref>
  : std::type_identity<Ret(Args..., ...) const &> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args...), volatile_lref>
  : std::type_identity<Ret(Args...) volatile &> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args..., ...), volatile_lref>
  : std::type_identity<Ret(Args..., ...) volatile &> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args...), cv_lref>
  : std::type_identity<Ret(Args...) const volatile &> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args..., ...), cv_lref>
  : std::type_identity<Ret(Args..., ...) const volatile &> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args...), rref>
  : std::type_identity<Ret(Args...) &&> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args..., ...), rref>
  : std::type_identity<Ret(Args..., ...) &&> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args...), const_rref>
  : std::type_identity<Ret(Args...) const &&> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args..., ...), const_rref>
  : std::type_identity<Ret(Args..., ...) const &&> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args...), volatile_rref>
  : std::type_identity<Ret(Args...) volatile &&> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args..., ...), volatile_rref>
  : std::type_identity<Ret(Args..., ...) volatile &&> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args...), cv_rref>
  : std::type_identity<Ret(Args...) const volatile &&> {};

template <class Ret, class... Args>
struct add_funcqual<Ret(Args..., ...), cv_rref>
  : std::type_identity<Ret(Args..., ...) const volatile &&> {};


template <class T, qualifier_id Q>
using add_qualifier_t = add_qualifier<T, Q>::type;

template <class T, qualifier_id Q>
using add_funcqual_t = add_funcqual<T, Q>::type;


template <class T>
using function_add_const = add_funcqual<T, const_value>;

template <class T>
using function_add_volatile = add_funcqual<T, volatile_value>;

template <class T>
using function_add_cv = add_funcqual<T, cv_value>;

template <class T>
using function_add_lref = add_funcqual<T, lref>;

template <class T>
using function_add_rref = add_funcqual<T, rref>;

template <class T>
using function_add_const_t = function_add_const<T>::type;

template <class T>
using function_add_volatile_t = function_add_volatile<T>::type;

template <class T>
using function_add_cv_t = function_add_cv<T>::type;

template <class T>
using function_add_lref_t = function_add_lref<T>::type;

template <class T>
using function_add_rref_t = function_add_rref<T>::type;


// reset qualifier

template <class T, qualifier_id Q>
using reset_qualifier = add_qualifier<qualifier_decay_t<T>, Q>;

template <class T, qualifier_id Q>
using reset_qualifier_t = reset_qualifier<T, Q>::type;

template <class T, qualifier_id Q>
using reset_funcqual = add_funcqual<funcqual_decay_t<T>, Q>;

template <class T, qualifier_id Q>
using reset_funcqual_t = reset_funcqual<T, Q>::type;


// remove qualifier

template <class T>
using remove_const = std::remove_const<T>;

template <class T>
using remove_volatile = std::remove_volatile<T>;

template <class T>
using remove_cv = std::remove_cv<T>;

template <class T>
using remove_ref = std::remove_reference<T>;

template <class T>
using remove_cvref = std::remove_cvref<T>;

template <class T>
using remove_const_t = std::remove_const_t<T>;

template <class T>
using remove_volatile_t = std::remove_volatile_t<T>;

template <class T>
using remove_cv_t = std::remove_cv_t<T>;

template <class T>
using remove_ref_t = std::remove_reference_t<T>;

template <class T>
using remove_cvref_t = std::remove_cvref_t<T>;

template <class T>
using function_remove_const =
  reset_funcqual<T, qualifier::remove_const(funcqual_traits_v<T>)>;

template <class T>
using function_remove_volatile =
  reset_funcqual<T, qualifier::remove_volatile(funcqual_traits_v<T>)>;

template <class T>
using function_remove_cv =
  reset_funcqual<T, qualifier::remove_cv(funcqual_traits_v<T>)>;

template <class T>
using function_remove_ref =
  reset_funcqual<T, qualifier::remove_ref(funcqual_traits_v<T>)>;

template <class T>
using function_remove_cvref =
  reset_funcqual<T, qualifier::remove_cvref(funcqual_traits_v<T>)>;

template <class T>
using function_remove_const_t = function_remove_const<T>::type;

template <class T>
using function_remove_volatile_t = function_remove_volatile<T>::type;

template <class T>
using function_remove_cv_t = function_remove_cv<T>::type;

template <class T>
using function_remove_ref_t = function_remove_ref<T>::type;

template <class T>
using function_remove_cvref_t = function_remove_cvref<T>::type;


// copy qualifier

template <class From, class To>
using copy_add_qualifier = add_qualifier<To, qualifier_traits_v<From>>;

template <class From, class To>
using copy_reset_qualifier = reset_qualifier<To, qualifier_traits_v<From>>;

template <class From, class To>
using copy_add_funcqual = add_funcqual<To, funcqual_traits_v<From>>;

template <class From, class To>
using copy_reset_funcqual = reset_funcqual<To, funcqual_traits_v<From>>;

template <class From, class To>
using copy_funcqual_add_qualifier =
  add_qualifier<To, funcqual_traits_v<From>>;

template <class From, class To>
using copy_funcqual_reset_qualifier =
  reset_qualifier<To, funcqual_traits_v<From>>;

template <class From, class To>
using copy_qualifier_add_funcqual =
  add_funcqual<To, qualifier_traits_v<From>>;

template <class From, class To>
using copy_qualfier_reset_funcqual =
  reset_funcqual<To, qualifier_traits_v<From>>;


template <class From, class To>
using copy_add_qualifier_t = copy_add_qualifier<From, To>::type;

template <class From, class To>
using copy_reset_qualifier_t = copy_reset_qualifier<From, To>::type;

template <class From, class To>
using copy_add_funcqual_t = copy_add_funcqual<From, To>::type;

template <class From, class To>
using copy_reset_funcqual_t = copy_reset_funcqual<From, To>::type;

template <class From, class To>
using copy_funcqual_add_qualifier_t =
  copy_funcqual_add_qualifier<From, To>::type;

template <class From, class To>
using copy_funcqual_reset_qualifier_t =
  copy_funcqual_reset_qualifier<From, To>::type;

template <class From, class To>
using copy_qualifier_add_funcqual_t =
  copy_qualifier_add_funcqual<From, To>::type;

template <class From, class To>
using copy_qualfier_reset_funcqual_t =
  copy_qualfier_reset_funcqual<From, To>::type;

}  // namespace xh

#endif  // !_XH_QUALIFIER_TRAITS_H_
