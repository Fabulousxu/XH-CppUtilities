#include <iostream>
#include <cassert>

#include "function_traits.h"
#include "function.h"
#include "cpp_tools.h"

using namespace std;

class A {
 public:
	int num;
	static const int k = 2;

	const xh::getter<int> getNumMoreThanK = [&]() { return num + k; };
	const xh::setter<int> setNumMoreThanK = [&](int n) { num = n - k; };
	const xh::getset<int, int> numMoreThanK = {
		[&]() { return num + k; },
		[&](int n) { num = n - k; }
	};
};

int main() {
	A a;
	a.num = 10;
	assert(a.getNumMoreThanK == 12);
	a.setNumMoreThanK = 13;
	assert(a.num == 11);
	assert(a.numMoreThanK == 13);






	// use auto_return to return a default value
	auto_return;
}