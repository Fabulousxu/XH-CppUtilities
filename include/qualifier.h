// XH-CppUtilities
// C++20 qualifier.h
// Author: xupeigong@sjtu.edu.cn
// Last Updated: 2024-09-12

#ifndef _XH_QUALIFIER_H_
#define _XH_QUALIFIER_H_

#include <type_traits>

namespace xh {
 
using namespace std;

struct qualifier {
  enum mask : uint8_t {
       const_mask = 0x01,
    volatile_mask = 0x02,
        lref_mask = 0x04,
        rref_mask = 0x08
  };

  enum id : uint8_t {
             value =           0x0,
       const_value =    const_mask,
    volatile_value = volatile_mask,
          cv_value = volatile_mask |    const_value,
              lref =     lref_mask,
        const_lref =     lref_mask |    const_value,
     volatile_lref =     lref_mask | volatile_value,
           cv_lref =     lref_mask |       cv_value,
              rref =     rref_mask,
        const_rref =     rref_mask |    const_value,
     volatile_rref =     rref_mask | volatile_value,
           cv_rref =     rref_mask |       cv_value
  };
};

struct funcqual {
  enum mask : uint8_t {
       const_mask = 0x01,
    volatile_mask = 0x02,
        lref_mask = 0x04,
        rref_mask = 0x08,
    noexcept_mask = 0x10
  };

  enum id : uint8_t {
                      value =           0x0,
                const_value =    const_mask,
             volatile_value = volatile_mask,
                   cv_value = volatile_mask |    const_value,
                       lref =     lref_mask,
                 const_lref =     lref_mask |    const_value,
              volatile_lref =     lref_mask | volatile_value,
                    cv_lref =     lref_mask |       cv_value,
                       rref =     rref_mask,
                 const_rref =     rref_mask |    const_value,
              volatile_rref =     rref_mask | volatile_value,
                    cv_rref =     rref_mask |       cv_value,
             value_noexcept = noexcept_mask |          value,
       const_value_noexcept = noexcept_mask |    const_value,
    volatile_value_noexcept = noexcept_mask | volatile_value,
          cv_value_noexcept = noexcept_mask |       cv_value,
              lref_noexcept = noexcept_mask |           lref,
        const_lref_noexcept = noexcept_mask |     const_lref,
     volatile_lref_noexcept = noexcept_mask |  volatile_lref,
           cv_lref_noexcept = noexcept_mask |        cv_lref,
              rref_noexcept = noexcept_mask |           rref,
        const_rref_noexcept = noexcept_mask |     const_rref,
     volatile_rref_noexcept = noexcept_mask |  volatile_rref,
           cv_rref_noexcept = noexcept_mask |        cv_rref
  };
};

using qualifier_mask = qualifier::mask;
using qualifier_id   = qualifier::id;
using funcqual_mask = funcqual::mask;
using funcqual_id   = funcqual::id;

template <class T>
struct qualifier_of : integral_constant<qualifier_id, qualifier_id::value> {
  using       type = T;
  using decay_type = T;
};

#define QUALIFIER_OF(cv, ref, id)                         \
  template <class T>                                      \
  struct qualifier_of<cv T ref>                           \
    : integral_constant<qualifier_id, qualifier_id::id> { \
    using       type = cv T ref;                          \
    using decay_type = T;                                 \
  };

QUALIFIER_OF(const, , const_value)
QUALIFIER_OF(volatile, , volatile_value)
QUALIFIER_OF(const volatile, , cv_value)
QUALIFIER_OF(, &, lref)
QUALIFIER_OF(const, &, const_lref)
QUALIFIER_OF(volatile, &, volatile_lref)
QUALIFIER_OF(const volatile, &, cv_lref)
QUALIFIER_OF(, &&, rref)
QUALIFIER_OF(const, &&, const_rref)
QUALIFIER_OF(volatile, &&, volatile_rref)
QUALIFIER_OF(const volatile, &&, cv_rref)

#undef QUALIFIER_OF

template <class T>
struct funcqual_of;

#define FUNCQUAL_OF(qual, id)                           \
  template <class Ret, class... Args>                   \
  struct funcqual_of<Ret(Args...) qual>                 \
    : integral_constant<funcqual_id, funcqual_id::id> { \
    using       type = Ret(Args...) qual;               \
    using decay_type = Ret(Args...);                    \
  };                                                    \
  template <class Ret, class... Args>                   \
  struct funcqual_of<Ret(Args..., ...) qual>            \
    : integral_constant<funcqual_id, funcqual_id::id> { \
    using       type = Ret(Args..., ...) qual;          \
    using decay_type = Ret(Args..., ...);               \
  };

FUNCQUAL_OF(, value)
FUNCQUAL_OF(const, const_value)
FUNCQUAL_OF(volatile, volatile_value)
FUNCQUAL_OF(const volatile, cv_value)
FUNCQUAL_OF(&, lref)
FUNCQUAL_OF(const&, const_lref)
FUNCQUAL_OF(volatile&, volatile_lref)
FUNCQUAL_OF(const volatile&, cv_lref)
FUNCQUAL_OF(&&, rref)
FUNCQUAL_OF(const&&, const_rref)
FUNCQUAL_OF(volatile&&, volatile_rref)
FUNCQUAL_OF(const volatile&&, cv_rref)
FUNCQUAL_OF(noexcept, value_noexcept)
FUNCQUAL_OF(const noexcept, const_value_noexcept)
FUNCQUAL_OF(volatile noexcept, volatile_value_noexcept)
FUNCQUAL_OF(const volatile noexcept, cv_value_noexcept)
FUNCQUAL_OF(& noexcept, lref_noexcept)
FUNCQUAL_OF(const& noexcept, const_lref_noexcept)
FUNCQUAL_OF(volatile& noexcept, volatile_lref_noexcept)
FUNCQUAL_OF(const volatile& noexcept, cv_lref_noexcept)
FUNCQUAL_OF(&& noexcept, rref_noexcept)
FUNCQUAL_OF(const&& noexcept, const_rref_noexcept)
FUNCQUAL_OF(volatile&& noexcept, volatile_rref_noexcept)
FUNCQUAL_OF(const volatile&& noexcept, cv_rref_noexcept)

#undef FUNCQUAL_OF

template <class T>
inline constexpr qualifier_id qualifier_of_v = qualifier_of<T>::value;
template <class T>
using qualifier_decay_t = qualifier_of<T>::decay_type;
template <class T>
inline constexpr funcqual_id funcqual_of_v = funcqual_of<T>::value;
template <class T>
using funcqual_decay_t = funcqual_of<T>::decay_type;

} // namespace xh

#endif // !_XH_QUALIFIER_H_
