#include <cstdarg>
#include <iostream>
#include "function_traits.h"

using std::cout;
using std::endl;
using std::is_same_v;
using namespace xh;

struct A {
	int operator()(int) const volatile &&{ return 0; }
};

int main() {
	const volatile A &&a{};
	cout << is_same_v<callable_traits_t<decltype(a)>, int(int)> << endl;
	return 0;
}
