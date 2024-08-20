# XH-CppTools

## Authors

- xupeigong@sjtu.edu.cn
- 1583913466@qq.com

## Overview

XH-CppTools is a collection of utility functions and classes for enhancing C++ functionality. It includes tools for function traits, getter and setter properties, advanced encapsulation of member functions, function overloading-like wrappers, chainable function calls, and more. This library is designed to extend C++ capabilities beyond what's offered by the standard library, with an emphasis on ease of use and flexibility.

## Environment

You need C++20 or a later version. The compiler can be GCC, MinGW, or MSVC. Other compilers have not been tested by the author.

## Features

- Function Traits: Offers introspection of function types and properties.
- Getter and Setter: Simplifies the creation of getter and setter properties in C++ classes.
- Member Function Wrapping: Provides utilities for better encapsulation and usage of member functions.
- Function Overloading: Implements multi-functionality to mimic function overloading in a more flexible way.
- Function Chaining: Allows chaining of functions for cleaner and more readable code.
- Pipe-like Function Calls: Supports pipe-like syntax for function execution.

## Files

- function_traits.h: Include function traits.
- function.h: Include getter and setter, member function, multi function, function chain and function pipe.
- cpp_tools.h: Include enum name and other cpp tools.
- tutorial.cpp: A simple tutorial for this project.

## Examples

In the `tutorial.cpp` file, we provide a concise yet comprehensive tutorial that covers various aspects of this project. Below, we showcase a few of the most important examples. To fully leverage these utilities and understand the underlying principles, please refer to the source code and tutorial in detail.

### xh::function_traits

Function traits allow you to introspect function types, making it easier to work with template metaprogramming. They are used to extract and obtain the composition information of function types, function pointers, member function pointers, functors, lambda expressions, and the cv-qualifiers and reference qualifiers for all these types. This includes the following member types:
 
| Member Type | Description |
| :---: | :---: |
| type | Type after removing reference and cv-qualifiers |
| class_type | The class type of a member function, with appropriate qualifiers added based on the member function's cv-qualifiers and reference qualifiers |
| return_type | Return type |
| argument_tuple | Parameter pack as std::tuple |
| template\<size_t N\> argument_type | Nth parameter type |
| arity | Number of parameters |

We also provide support for variadic functions. When extracting function type traits for variadic functions, `argument_tuple` contains only the parameter pack of the fixed arguments, `argument_type` only supports looking up types for fixed arguments and `arity` represents the number of fixed parameters.

```C++
#include "function_traits.h"

int fun_0(int) { return 0; }
class A {
 public:
  int num;
  int add(int n) volatile {
    num += n;
    return num;
  }
};

int main() {
  const auto &&fun_1 = [](int) { return 0; };
  static_assert(std::is_same_v<xh::function_traits_t<decltype(&fun_0)>, int(int)>);
  static_assert(std::is_same_v<xh::function_traits_t<decltype(fun_1)>, int(int)>);
  static_assert(std::is_same_v<xh::function_traits_t<decltype(&A::add)>, int (A::*)(int) volatile>);
  static_assert(std::is_same_v<xh::function_class_t<decltype(&A::add)>, A volatile>);

  return 0;
}
```

### xh::getter, xh::setter, xh::getset

The `getter` and `setter` utilities simplify the creation of class properties that perform custom actions when getting or setting a value. In a class, you can define members of `getter` and `setter` types, which overload the type conversion and assignment operators, respectively. When accessing a `getter` member, a custom getter function is called to obtain the return value, while assigning to a `setter` member triggers a custom setter function to modify the value. The `getter` and `setter` types are constructed by passing a callable object, while the `getset` type is constructed by passing two callable objects.

```C++
#include <cassert>
#include "function.h"

class A {
 public:
  int num;
  static const int k = 2;
  A(int n) : num(n) {}

  const xh::getter<int> getNumMoreThanK = [&]() { return num + k; };
  const xh::setter<int> setNumMoreThanK = [&](int n) { num = n - k; };
  const xh::getset<int, int> numMoreThanK = {
    [&]() { return num + k; },
    [&](int n) { num = n - k; }
  };
};

int main() {
  A a = 10;
  assert(a.getNumMoreThanK == 12);
  a.setNumMoreThanK = 13;
  assert(a.num == 11);
  assert(a.numMoreThanK == 13);
  a.numMoreThanK = 14;
  assert(a.num == 12);

  return 0;
}
```

### xh::member_function

The `member_function` template wraps member functions, making them easier to pass around and use as first-class objects. It achieves this by overloading the function call operator `()`, allowing member functions to be invoked with a pointer or reference to an object of the class as the first argument. The wrapper adapts to different cv-qualifiers and reference qualifiers of the member function, ensuring appropriate handling of the first argument based on these qualifiers.

```C++
#include <cassert>
#include "function.h"

class A {
 public:
  int num;
  A(int n) : num(n) {}

  int add(int n) volatile {
    num += n;
    return num;
  }
}

int main() {
  A a(12);
  xh::member_function mf = &A::add;
  assert(mf(a, 2) == 14);
  mf(&a, 2);
  assert(a.num == 16);

  return 0;
}
```

### MEMBER_FUNCTION_PROXY

`MEMBER_FUNCTION_PROXY` allows you to create a proxy function for a class member function. This proxy can intercept calls and modify behavior.

```C++
#include <cassert>
#include "function.h"

class A {
 public:
  int num;
}

int main() {
  int (A::*ptr)(int);
  MEMBER_FUNCTION_PROXY(ptr, (int n), {
    proxy->num += n;
    return proxy->num;
  });
  a.num = 10;
  assert((a.*ptr)(2) == 12);

  return 0;
}
```

### xh::multi_function

The `multi_function` template allows the creation of a function object that can behave like an overloaded function, handling multiple types of arguments. It lets you pass multiple callable objects at compile-time and overloads the function call operator `()`, enabling the appropriate callable to be invoked based on the provided arguments.
 
```C++
#include <cassert>
#include "function.h"

int reload_fun(int) { return 0; }
char reload_fun(char) { return 'a'; }

int main() {
  xh::multi_function multi_fun = {
      [](int) { return 0; },
      [](char) { return 'a'; }
  };
  assert(multi_fun(1) == reload_fun(1));
  assert(multi_fun('a') == reload_fun('a'));
  assert(multi_fun((short)'a') == reload_fun((short)'a'));

  return 0;
}
```

### xh::function_chain

`function_chain` allows you to chain multiple functions together, passing the result of one as the input to the next.
 
```C++
#include <cassert>
#include "function.h"

int main() {
  xh::function_chain([](int a, int b) { return a + b; })
      .then([](int a) { return a * 2; })
      .then([](int res) { assert(res == (1 + 2) * 2); })
          (1, 2);

	return 0;
}
```

### xh::function_pipe

The `function_pipe` allows functions to be called using a syntax similar to a pipe operator. It supports chaining function calls in a manner similar to `function_chain`, where the output of each function becomes the first argument of the next function in the pipeline.

```C++
#include <cassert>
#include "function.h"

int main(){
  xh::function_pipe pf1 = [](int a, int b) { return a + b; };
  xh::function_pipe pf2 = [](int a) { return a * 2; };
  xh::function_pipe pf3 = [](int res) { assert(res == (1 + 2) * 2); };
  1 | pf1(2) | pf2 | pf3;

  return 0;
}
```