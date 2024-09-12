#include <any>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <set>
#include <thread>
#include <type_traits>
#include <vector>

#include "function_traits.h"
#include "function_utility.h"
#include "qualifier.h"

using namespace xh;
using namespace std;

int fun(int) { return 0; }
char cfun(int) { return 'a'; }

int main() {
//  xh::multifunc f = {[](int) { return 0; }, [](char) { return 1; }};
//
//  cout << f(1) << endl;
//  cout << f('a') << endl;

xh::function ff = [](int) { return 0; };

cout << ff(1);

  return 0;
}
