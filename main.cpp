#include <cstdarg>
#include <iostream>
#include <functional>
#include "function_traits.h"
#include "function_tools.h"
#include "function_test.h"
#include "cpp_tools.h"

using namespace xh;

USE_STD_IO;

struct A {
	int count = 1;
	get(getCount, int, {
		int a = 1, b = 2;
		return a + b;
	})
	set(setCount, int, val, {
		this->count = val;
	})

	getset(moreThanCount, int, ({
		return this->count + 1;
	}), int, val, ({
		this->count = val - 1;
	}))
};

int main() {
	A a;
	a.count = 1;
	cout << a.moreThanCount << endl;
	a.moreThanCount = 11;
	cout << a.moreThanCount << endl;

	auto_return;
}
