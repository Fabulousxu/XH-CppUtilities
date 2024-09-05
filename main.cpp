#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <thread>
#include <vector>

// #include "cpp_tools.h"
// #include "function_tools.h"
// #include "function.h"

#include "function_traits.h"
#include "qualifier_traits.h"

using namespace xh;
using namespace std;

#if 1

int fun(int) { return 0; }
int (*ptr)(int) = fun;
auto lambda = [](int) { return 0; };
struct A {
  int fun(int) const { return 0; };
};

int main() {
  const auto &f = &A::fun;
  cout << is_same_v<memfunc_func_decay_t<decltype(f)>, int(int)> << endl;

  return 0;
}

#endif
