#ifndef ENUM_NAME_H
#define ENUM_NAME_H

#include <string_view>
#include <array>
#include <iostream>

namespace xh {

template<auto>
constexpr auto enum_name() {
#if __GNUC__ || __clang__
  std::string_view name = __PRETTY_FUNCTION__;
  size_t start = name.find('=') + 2, end = name.size() - 1;
#elif _MSC_VER
  std::string_view name = __FUNCSIG__;
  size_t start = name.find('<') + 1, end = name.rfind('>');
#endif
  name = std::string_view{name.data() + start, end - start};
  if ((start = name.rfind("::")) != std::string_view::npos)
    name = std::string_view{name.data() + start + 2, name.size() - start - 2};
  if (name.find(')') != std::string_view::npos) name = "";
  return name;
}

template<typename T, size_t N = 0>
constexpr auto enum_max() {
  if constexpr (!enum_name<static_cast<T>(N)>().empty())
    return enum_max<T, N + 1>();
  return N;
}

template<typename T>
constexpr auto enum_name(T value) {
  constexpr auto num = enum_max<T>();
  constexpr auto names = []<size_t... N>(std::index_sequence<N...>) {
    return std::array<std::string_view, num>{enum_name<static_cast<T>(N)>()...};
  }(std::make_index_sequence<num>{});
  return names[static_cast<size_t>(value)];
}

}; // namespace xh

#endif //ENUM_NAME_H
