#include <cassert>
#include <iostream>

#include "cpp_tools.h"
#include "function.h"
#include "function_traits.h"

using namespace xh;
using namespace std;

int main() {
	function_chain([](int a, int b) { return a + b; })
		.then([](int a) { return a * 2; })
		.then([](int a) { cout << a << endl; })
			(1, 2);

	auto_return;
}
