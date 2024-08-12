#include <cstdarg>
#include <iostream>
#include "enum_name.h"
#include "function.h"
#include "promise.h"

using std::cout;
using std::endl;
using std::is_same_v;
using namespace xh;

int fun_0(int) { return 0; }
auto &fun_1 = fun_0;
auto &&fun_2 = std::move(fun_0);
const auto fun_3 = fun_0;
const auto &fun_4 = fun_0;
const auto &&fun_5 = std::move(fun_0);
volatile auto fun_6 = fun_0;
volatile auto &fun_7 = fun_0;
volatile auto &&fun_8 = std::move(fun_0);
const volatile auto fun_9 = fun_0;
const volatile auto &fun_10 = fun_0;
const volatile auto &&fun_11 = std::move(fun_0);
auto *fun_12 = &fun_0;
auto &fun_13 = fun_12;
auto &&fun_14 = std::move(fun_12);
const auto fun_15 = fun_12;
const auto &fun_16 = fun_12;
const auto &&fun_17 = std::move(fun_12);
volatile auto fun_18 = fun_12;
volatile auto &fun_19 = fun_12;
volatile auto &&fun_20 = std::move(fun_12);
const volatile auto fun_21 = fun_12;
const volatile auto &fun_22 = fun_12;
const volatile auto &&fun_23 = std::move(fun_12);
auto fun_24 = [](int...) { return 0; };
auto &fun_25 = fun_24;
auto &&fun_26 = std::move(fun_24);
const auto fun_27 = fun_24;
const auto &fun_28 = fun_24;
const auto &&fun_29 = std::move(fun_24);
volatile auto fun_30 = fun_24;
volatile auto &fun_31 = fun_24;
volatile auto &&fun_32 = std::move(fun_24);
const volatile auto fun_33 = fun_24;
const volatile auto &fun_34 = fun_24;
const volatile auto &&fun_35 = std::move(fun_24);

struct A {
  int fun(int) { return 0; }
};

int main() {
  cout << is_same_v<callable_traits_t<decltype(fun_0)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_1)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_2)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_3)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_4)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_5)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_6)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_7)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_8)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_9)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_10)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_11)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_12)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_13)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_14)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_15)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_16)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_17)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_18)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_19)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_20)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_21)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_22)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_23)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_24)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_25)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_26)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_27)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_28)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_29)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_30)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_31)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_32)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_33)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_34)>, int(int)> << endl;
  cout << is_same_v<callable_traits_t<decltype(fun_35)>, int(int)> << endl;
  return 0;
}
