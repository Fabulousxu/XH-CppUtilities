#include <iostream>
#include "Promise.h"
#include "enum_name.h"

int main() {
  enum Color { RED = 0, GREEN = 1, BLUE = 2 };
  Color c = (Color)0;
  std::cout << enum_name(c) << std::endl;
  return 0;
}
