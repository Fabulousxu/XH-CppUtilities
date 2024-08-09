#include <iostream>
#include "promise.h"
#include "enum_name.h"
#include "member_function.h"

struct A {
  int add(int a, int b) { return a + b; }
  int sub(int a, int b) const { return a - b; }
  int mul(int a, int b) volatile { return a * b; }
  int div(int a, int b) const volatile { return a / b; }
};

int main() {
  xh::mem_fun add = &A::add;
  xh::mem_fun sub = &A::sub;
  xh::mem_fun mul = &A::mul;
  xh::mem_fun div = &A::div;
  A a;
  const A b;
  volatile A c;
  const volatile A d;

  add(a, 2, 1);
  sub(b, 2, 1);
  mul(c, 2, 1);
  div(d, 2, 1);

  return 0;
}
