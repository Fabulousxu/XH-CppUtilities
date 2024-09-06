#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <set>
#include <thread>
#include <vector>

#include "function_traits.h"
#include "function_utility.h"
#include "qualifier.h"

using namespace xh;
using namespace std;

struct A {
  int fun(int) { return 0; }
};

int main() {
  auto f = std::mem_fn(&A::fun);
  A a;
  cout << f(&a, 1);

  return 0;
}
