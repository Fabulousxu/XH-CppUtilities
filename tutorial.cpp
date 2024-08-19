// C++20 function.h
// Author: xupeigong@sjtu.edu.cn, 1583913466@qq.com
// Last Updated: 2024-08-19

// This file is a tutorial for the XH-CppTools.

#ifndef _XH_TUTORIAL_CPP_
#define _XH_TUTORIAL_CPP_

#include <cassert>
#include "function_traits.h"
#include "function.h"
#include "cpp_tools.h"

// inherit from comparable to implement the compare function
class A : public xh::comparable<A> {
 public:
  int num;

  static const int k = 2;

  A(int n) : num(n) {}

  // generate default constructors and destructor for your class
  NO_ARGUMENT_CONSTRUCTOR(A)
  COPY_CONSTRUCTOR(A)
  MOVE_CONSTRUCTOR(A)
  DESTRUCTOR(A)

  // initialize getter and setter in your class
  const xh::getter<int> getNumMoreThanK = [&]() { return num + k; };
  const xh::setter<int> setNumMoreThanK = [&](int n) { num = n - k; };
  const xh::getset<int, int> numMoreThanK = {
      [&]() { return num + k; },
      [&](int n) { num = n - k; }
  };

  // implement the compare function
  int compare(const A &obj) const override { return num - obj.num; }

  int add(int n) volatile {
    num += n;
    return num;
  }
};

int reload_fun(int) { return 0; }
char reload_fun(char) { return 'a'; }

int main() {
  A a = 10;

  // use getter and setter to get or set some value after processed
  assert(a.getNumMoreThanK == 12);
  a.setNumMoreThanK = 13;
  assert(a.num == 11);
  assert(a.numMoreThanK == 13);
  a.numMoreThanK = 14;
  assert(a.num == 12);

  // use member_function to wrap a member function
  xh::member_function mf = &A::add;
  assert(mf(a, 2) == 14);
  mf(&a, 2);
  assert(a.num == 16);

  // use MEMBER_FUNCTION_PROXY to create a proxy for a member function
  int (A::*ptr)(int);
  MEMBER_FUNCTION_PROXY(ptr, (int n), {
    proxy->num += n;
    return proxy->num;
  });
  a.num = 10;
  assert((a.*ptr)(2) == 12);

  // use multi_function to create a reload-like function
  xh::multi_function multi_fun = {
      [](int) { return 0; },
      [](char) { return 'a'; }
  };
  assert(multi_fun(1) == reload_fun(1));
  assert(multi_fun('a') == reload_fun('a'));
  assert(multi_fun((short)'a') == reload_fun((short)'a'));

  // use function_chain to call a chain of functions
  xh::function_chain([](int a, int b) { return a + b; })
      .then([](int a) { return a * 2; })
      .then([](int res) { assert(res == (1 + 2) * 2); })
          (1, 2);

  // function traits
  auto fun_0 = [](int) { return 0; };
  const auto &&fun_1 = [](int) { return 0; };
  static_assert(std::is_same_v<xh::function_traits_t<decltype(fun_0)>,
                               int(int)>);
  static_assert(std::is_same_v<xh::function_traits_t<decltype(fun_1)>,
                               int(int)>);
  static_assert(std::is_same_v<xh::function_traits_t<decltype(&A::add)>,
                               int (A::*)(int) volatile>);
  static_assert(std::is_same_v<xh::function_class_t<decltype(&A::add)>,
                               A volatile>);

  // use auto_return to return a default value
  auto_return;
}

#endif //_XH_TUTORIAL_CPP_
