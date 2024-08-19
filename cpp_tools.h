// C++20 function.h
// Author: xupeigong@sjtu.edu.cn, 1583913466@qq.com
// Last Updated: 2024-08-19

// This header file defines several utility components and macros, expanding on
// certain standard functionalities. It includes features for obtaining the
// names of enum values at compile-time, implementing an auto_return mechanism,
// generating default constructors and assignment operators, and providing a
// comparable interface for creating objects that can be compared using
// standard comparison operators.

#ifndef _XH_CPP_TOOLS_H_
#define _XH_CPP_TOOLS_H_

#include <string_view>
#include "function_traits.h"

using namespace std;

namespace xh {

/*************
 * enum name *
 *************/

template<auto>
constexpr auto enum_name() {
#if __GNUC__ || __clang__
  string_view name = __PRETTY_FUNCTION__;
  size_t start = name.find('=') + 2, end = name.size() - 1;
#elif _MSC_VER
  string_view name = __FUNCSIG__;
  size_t start = name.find('<') + 1, end = name.rfind('>');
#endif
  name = string_view{name.data() + start, end - start};
  if ((start = name.rfind("::")) != string_view::npos)
    name = string_view{name.data() + start + 2, name.size() - start - 2};
  return name.find(')') == string_view::npos ? name : "";
}

template<typename T, size_t N = 0>
constexpr auto enum_max() {
  if constexpr (!enum_name<static_cast<T>(N)>().empty())
    return enum_max<T, N + 1>();
  return N;
}

template<typename T> requires is_enum_v<T>
constexpr auto enum_name(T value) {
  constexpr auto num = enum_max<T>();
  constexpr auto names = []<size_t... N>(std::index_sequence<N...>) {
    return std::array<std::string_view, num>{enum_name<static_cast<T>(N)>()...};
  }(std::make_index_sequence<num>{});
  return names[static_cast<size_t>(value)];
}


/***************
 * auto return *
 ***************/

struct auto_return_t {
  template<class T>
  operator T() const { return T{}; }
};

#define auto_return return xh::auto_return_t{};


/**********************************************************************
 * generate default constructors, destructor and assignment operators *
 **********************************************************************/

#define NO_ARGUMENT_CONSTRUCTOR(class_name) \
  class_name() = default;

#define DESTRUCTOR(class_name) \
  ~class_name() = default;

#define COPY_CONSTRUCTOR(class_name) \
  class_name(const class_name &) = default;

#define MOVE_CONSTRUCTOR(class_name) \
  class_name(class_name &&) = default;

#define COPY_ASSIGNMENT_OPERATOR(class_name) \
  class_name &operator=(const class_name &) = default;

#define MOVE_ASSIGNMENT_OPERATOR(class_name) \
  class_name &operator=(class_name &&) = default;


/************************
 * comparable interface *
 ************************/

template<class Derived>
class comparable {
 public:
  virtual int compare(const Derived &obj) const = 0;
  virtual bool operator<(const Derived &obj) const
  { return compare(obj) < 0; }
  virtual bool operator<=(const Derived &obj) const
  { return compare(obj) <= 0; }
  virtual bool operator==(const Derived &obj) const
  { return !compare(obj); }
  virtual bool operator!=(const Derived &obj) const
  { return compare(obj); }
  virtual bool operator>=(const Derived &obj) const
  { return compare(obj) >= 0; }
  virtual bool operator>(const Derived &obj) const
  { return compare(obj) > 0; }
};

};

#endif //_XH_CPP_TOOLS_H_
