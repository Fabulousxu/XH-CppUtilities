// XH-CppUtilities
// C++20 enum_name.h
// Author: xupeigong@sjtu.edu.cn
// Last Updated: 2024-09-12

#ifndef _XH_ENUM_NAME_H_
#define _XH_ENUM_NAME_H_

#include <string_view>
#include <type_traits>

namespace xh {

using namespace std;

// enum name

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
  constexpr auto names = []<size_t... N>(index_sequence<N...>) {
    return array<string_view, num>{enum_name<static_cast<T>(N)>()...};
  }(make_index_sequence<num>{});
  return names[static_cast<size_t>(value)];
}

};

#endif //_XH_ENUM_NAME_H_
