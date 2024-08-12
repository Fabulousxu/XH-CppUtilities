#include <iostream>
#include <cstdarg>
#include "promise.h"
#include "enum_name.h"
#include "function.h"

using std::cout;
using std::endl;
using std::is_same_v;
using namespace xh;

int sum(int a...) { return 0; }

int main() {
  cout << std::is_function_v<decltype([](int a){return a;})> << endl;

  return 0;
}
