#ifndef XH_FUNCTION_TEST_H
#define XH_FUNCTION_TEST_H

#include <utility>

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
auto fun_24 = [](int) { return 0; };
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

#endif //XH_FUNCTION_TEST_H
