#include <cassert>
#include <iostream>

#include "cpp_tools.h"
#include "function.h"
#include "function_traits.h"

using namespace xh;
using namespace std;

int add(int a, int b) { return a + b; }

int main() {
	auto add1 = [](int x) { return x + 1; };
	auto addString1 = [](string s) { return s + "1"; };

	multi_function fun(add, add1, addString1);

	cout << fun(1) << endl;
	cout << fun(1, 2) << endl;
	cout << fun("a") << endl;

	multi_functor fun2(add1, addString1);

	cout << fun2(1) << endl;
	cout << fun2("a") << endl;

	auto_return;
}
