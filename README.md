# XH-CppTools

## xh::function_traits

```C++
#include "function_traits.h"

template<class _T>
struct function_traits;
```

函数类型萃取，用于获取可调用类型的返回以及参数等类型 

成员类型： 

| 类型 | 说明 |
| :---: | :---: |
| type | 可调用类型去除引用限定以及cv限定后的类型 |
| return_type | 返回值类型 |
| std_type | 相对应的std::function |
| argument_tuple | 参数包的std::tuple |
| template\<size_t _N> argument_type | 第N个参数类型 |
| arity | 参数个数 |

其中，对不定参数函数进行函数类型萃取时，仅包含成员类型type和return_type 
可调用类型包括函数类型、函数指针类型、仿函数类型、匿名函数类型，以及上述所有类型的cv限定与引用类型，可以使用**xh:is_callable**判断是否为可调用类型。 

示例代码：

```C++
#include "function_traits.h"

void fun(int) {}

int main() {
  static_assert(std::is_same_v<function_traits<decltype(&fun)>::type, void(int)>);
  static_assert(std::is_same_v<function_traits<decltype(&fun)>::return_type, void>)
}
```


## get, set, getset

```C++
#include "function_tools.h"

#define get(name, return_type, ...)
#define set(name, argument_type, argument_name, ...)
#define getset(name, return_type, get, argument_type, argument_name, set)
```
通过get、set、getset三个宏，可以定义getter和setter属性。 

示例代码： 
```C++
#include <cassert>
#include "function_tools.h"

struct A {
  int a;
  get(getA, int, { return a; })
  set(setA, int, val, { a = val; })
  getset(getsetA, int, ({ return a; }), int, val, ({ a = val; }))
};

int main() {
  A a;
  a.setA = 1
  assert(a.getA == 1);
  a.getsetA = 2;
  assert(a.getsetA == 2);
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
