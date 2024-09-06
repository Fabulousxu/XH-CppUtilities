#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <thread>
#include <vector>

#include "function_traits.h"
#include "function_utility.h"
#include "qualifier_traits.h"

using namespace xh;
using namespace std;

int main() { 
  auto f1 = [](int a, int b) { return a + b; };
  auto f2 = [](int a, int b) { return a + b; };
  auto f3 = [](int a) { return a * 2; };
  auto f4 = [](int a) { cout << a; };

funcchain(f1).then(f3).then(f4)(1, 2);

  return 0;
}
