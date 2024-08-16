#include <iostream>
#include <cassert>

#include "function_traits.h"
#include "function.h"
#include "cpp_tools.h"

using namespace std;

#if 1

struct A {
	int num;
	static const int k = 2;

	// initialize getter and setter in your class
	const xh::getter<int> getNumMoreThanK = [&]() { return num + k; };
	const xh::setter<int> setNumMoreThanK = [&](int n) { num = n - k; };
	const xh::getset<int, int> numMoreThanK = {
		[&]() { return num + k; },
		[&](int n) { num = n - k; }
	};
};

int main() {
	A a{.num = 10};
	// use getter and setter to get or set some value after processed
	assert(a.getNumMoreThanK == 12);
	a.setNumMoreThanK = 13;
	assert(a.num == 11);
	assert(a.numMoreThanK == 13);
	a.numMoreThanK = 14;
	assert(a.num == 12);

	// use MEMBER_FUNCTION_PROXY to create a proxy for a member function
	int (A::*ptr)(int);
	MEMBER_FUNCTION_PROXY(ptr, (int n) -> int, {
		proxy->num += n;
		return proxy->num;
	});
	assert((a.*ptr)(2) == 14);

	// use multi_function to create a function like the reload function
	xh::multi_function multi_fun = {
		[](int) { return 0; },
		[](char) { return 'a'; }
	};
	assert(multi_fun(1) == 0);
	assert(multi_fun('a') == 'a');
	assert(multi_fun((short)'a') == 0);

	// use auto_return to return a default value
	auto_return;
}

#endif