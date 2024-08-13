#include <iostream>
#include <cassert>
#include "cpp_tools.h"
#include "function_traits.h"
#include "function_tools.h"

using namespace xh;

USE_STD_IO;

struct A {
	int a;
	const get<int> getA = [this] { return a; };
	set<int> setA = [this](int v) { a = v; };
	getset<int, int> getSetA = {
		[this] { return a; },
		[this](int v) { a = v; }
	};
};

int main() {

	auto_return;
}
