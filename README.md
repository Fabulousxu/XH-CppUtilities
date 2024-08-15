# XH-CppTools

本项目实现了一些对c++一些小功能的扩展，需使用c++20及以上版本，敬请使用。以下是对几个重要功能的说明。详细的全部功能详见几个头文件，内含说明。

## xh::function_traits
```C++
#include "function_traits.h"

template<class T>
struct function_traits;
```
函数类型萃取，用于获取函数、函数指针、成员函数指针、仿函数、匿名函数及以上所有类型的cv限定类型和引用限定类型的组成信息，包括以下成员类型： 
| 成员类型 | 说明 |
| :---: | :---: |
| type | 去除引用限定和cv限定后的类型 |
| class_type | 成员函数的类，根据成员函数的cv限定和引用限定而添加相应限定后的类型 |
| return_type | 返回值类型 |
| argument_tuple | 参数包的std::tuple |
| template\<size_t N\> argument_type | 第N个参数类型 |
| arity | 参数个数 |

其中，对不定参数函数进行函数类型萃取时，argument_tuple仅是确定参数的参数包，argument_t仅支持查找确定参数的类型，arity是确定参数的个数。 
 
示例代码：
```C++
#include "function_traits.h"

void fun(int) {}
struct A { void fun(int) const {} };

int main() {
  static_assert(std::is_same_v<xh::function_traits<decltype(&fun)>::type, void(int)>);
  static_assert(std::is_same_v<xh::function_traits<decltype(&A::fun)>::class_type, const A>);
  return 0;
}
```

## xh::getter, xh::setter, xh::getset
```C++
#include "function.h"

template<class Ret>
class getter;

template<class Arg>
class setter;

template<class Ret, class Arg>
class getset;
```
在类中可以定义getter和setter类型的成员。分别重载了类型转换与赋值运算符。在获取getter类型成员时，会调用自定义getter函数，获取到返回值；在赋值setter类型成员时，会调用自定义setter函数，对一些值进行修改。其中getter和setter传入一个可调用对象构造，getset传入两个可调用对象构造。 
 
示例代码： 
```C++
#include <cassert>
#include "function.h"

struct A {
  int a;
  const xh::getter<int> getA = [this] { return a; };
  const xh::setter<int> setA = [this](int val) { a = val; };
  const xh::getset<int, int> getsetA = {
    [this] { return a; },
    [this](int val) { a = val; }
  };
};

int main() {
  A a;
  a.setA = 1;
  assert(a.getA == 1);
  a.getsetA = 2;
  assert(a.getsetA == 2);
  return 0;
}
```

## xh::member_function
```C++
#include "function.h"

template<class T>
class member_function;
```
成员函数包装类，重载了括号运算符，支持第一个参数传入类的对象的指针或引用，进行调用成员函数。根据成员函数的cv限定和引用限定，对第一个参数的传入做了不同的适配。 
 
示例代码：
```C++
#include "function.h"

struct A {
  void fun1(int) {}
  void fun2(int, int) && {}
}

int main() {
  xh::member_function f1 = &A::fun1;
  xh::member_function f2 = &A::fun2;
  A a;
  f1(&a, 1);
  f2(A(), 1, 2);
  return 0;
}
```

## xh::multi_function
```C++
#include "function.h"

template<class... T>
class multi_function;
```
类似函数重载，需在编译期传入若干个可调用对象。重载了括号运算符，支持对所传入的函数进行对应地调用。 
 
示例代码：
```C++
#include <string>
#include "function.h"

void fun1(int) {}

int main() {
	auto fun2 = [](int x, int y) { return x + y; };
	auto fun3 = [](std::string s) { return s + "1"; };

	multi_function fun(fun1, fun2, fun3);
  fun(1);
  fun(1, 2);
  fun("a");
  return 0;
}
```

## xh::function_chain
```C++
#include "function.h"

template<class T>
class function_chain;
```
函数调用链，支持上一个函数的返回值作为下一个函数的参数，链式调用函数。 
 
示例代码：
```C++
#include <iostream>
#include "function.h"

int main() {
	function_chain([](int a, int b) { return a + b; })
		.then([](int a) { return a * 2; })
		.then([](int a) { cout << a << endl; })
			(1, 2);
	return 0;
}
```

## xh::predicate

Something like C++20 pipe operator. Through our *predicate*, you can convert any function-like things into "pipe-operatable".

U can also take this as "dynamic add (public) method to a class". Just can't visit private or protected things.

```C++
#include "predicate.h"
#include <vector>
#include <cstdio>
void print_func(const std::vector<int> &vec){
    for(auto &v:vec){
        printf("%d\n", v);
    }
    printf("print1\n");
}
int main(){
    std::vector<int> v{1,2,3};
    int a=1;
    auto print2 = PRED(
        [&a](const std::vector<int> &vec){
            for(auto &v:vec){
                printf("%d,\n",v+a);
            }
            printf("print2\n");
        });
    auto print1 = PRED(print_func);
    v|print1; 
    /*
        if there's no other arguments, 
        we allow you not to type () 
    */
    v|print2();
}
```

## ADD_METHOD

This is another way we tried to add method to class dynamically. 

```C++
#include "test_add_method.h"
#include <iostream>
struct Test{
    int b;
    Test(int b_=1):b(b_){}
};
int main(){
    int (Test::* ptr)(int) const = nullptr;
    ADD_METHOD(ptr, (int a) const {
       const Test *self = (const Test*)this;
       return a+self->b;
    })
    Test obj;
    std::cout<<(obj.*ptr)(1);
}
```

Still, we can't visit private and protected things.
