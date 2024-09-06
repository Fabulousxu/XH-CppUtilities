// XH-CppUtilities
// C++20 utility.h
// Author: xupeigong@sjtu.edu.cn, 1583913466@qq.com
// Last Updated: 2024-08-19

#ifndef _XH_UTILITY_H_
#define _XH_UTILITY_H_

#include <string_view>
#include <type_traits>

namespace xh {

// enum name

template<auto>
constexpr auto enum_name() {
#if __GNUC__ || __clang__
  std::string_view name = __PRETTY_FUNCTION__;
  std::size_t start = name.find('=') + 2, end = name.size() - 1;
#elif _MSC_VER
  std::string_view name = __FUNCSIG__;
  std::size_t start = name.find('<') + 1, end = name.rfind('>');
#endif
  name = std::string_view{name.data() + start, end - start};
  if ((start = name.rfind("::")) != std::string_view::npos)
    name = std::string_view{name.data() + start + 2, name.size() - start - 2};
  return name.find(')') == std::string_view::npos ? name : "";
}

template<typename T, size_t N = 0>
constexpr auto enum_max() {
  if constexpr (!enum_name<static_cast<T>(N)>().empty())
    return enum_max<T, N + 1>();
  return N;
}

template<typename T> requires std::is_enum_v<T>
constexpr auto enum_name(T value) {
  constexpr auto num = enum_max<T>();
  constexpr auto names = []<std::size_t... N>(std::index_sequence<N...>) {
    return std::array<std::string_view, num>{enum_name<static_cast<T>(N)>()...};
  }(std::make_index_sequence<num>{});
  return names[static_cast<std::size_t>(value)];
}

// auto return

struct auto_return_t {
  template<class T>
  operator T() const { return T{}; }
};

#define auto_return return xh::auto_return_t{};

};

#endif //_XH_UTILITY_H_
