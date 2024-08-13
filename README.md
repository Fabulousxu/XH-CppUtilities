# CppHelper
## callable_traits
How can we get the return type and arguments' type **DIRECTLY** from a function-like type? Use our *callable_traits*!
```C++
#include "function_traits.h"
#include <iostream>
int func1(int){return 0;}
int main(){
    const auto v1 = std::is_same_v<
        xh::callable_traits_t<decltype(&func1)>, 
        int(int)>;
    const auto v2 = std::is_same_v<
        xh::callable_return_t<decltype(&func1)>,
        int
    >;
    printf("%d,%d,%d\n", v1,v2,v3); //1,1,1
    return 0;
}
```
To get arguments' type, you can use *callable_argument_tuple\<Func\>*. For one exact argument's type, use *callable_argument_type<Func\,0>*.

Lambda or any callable function-like things are all accepted. But notice that member function (aka method) is not accepted. 

## enum_name
This function can help you get the name of one enumarate value as string_view.
```C++
#include "enum_name.h"
#include <iostream>
enum Color{
    RED=0,
    BLUE=1,
    GREEN=2
};
int main(){
    Color c = RED;
    std::cout<<xh::enum_name(c)<<std::endl; 
    //output "RED"
    return 0;
}
```
## predicate
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
## getter and setter
Like the getter and setter of JavaScript, we found a way to register getter and setter in C++.
```C++
#include "cpp_helper.h"
#include <iostream>
USE_STD_IO
class T{
    int a;
public:
    T(int a_=10):a(a_){}
    get(half_a, double){return a/2.0;}
    set(half_a, double){this->a = 2*arg;}
};
int main(){
    T obj(100);
    obj.halfa = 55.5;
    cout<<obj.halfa; //output 111
    return 0; 
}
```
## promise
