#include <iostream>
#include "Promise.h"

int main() {
  Promise([](int x) { return x; })
      .then([](int x) { std::cout << x; })
          (1);
  return 0;
}
