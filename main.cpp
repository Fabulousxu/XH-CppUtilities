#include <iostream>
#include "promise.h"
#include "enum_name.h"
#include "function.h"

struct A {
  int add(int a, int b) { return a + b; }
  int sub(int a, int b) const { return a - b; }
  int mul(int a, int b) volatile { return a * b; }
  int div(int a, int b) const volatile { return a / b; }
};

//===========================mem_fun===========================
int mem_fun_example() {
  xh::mem_fun add = &A::add;
  xh::mem_fun sub = &A::sub;
  xh::mem_fun mul = &A::mul;
  xh::mem_fun div = &A::div;
  A a;
  const A b;
  volatile A c;
  const volatile A d;

  add(a, 2, 1);
  sub(b, 2, 1);
  mul(c, 2, 1);
  div(d, 2, 1);

  return 0;
}


//===========================pred===========================
#include <vector>
#include "predicate.h"
std::vector<int> only_odd(std::vector<int> &vec){
  std::vector<int> res{};
  for(auto &num:vec){
    if(num&1)
      res.push_back(num);
  }
  return res;
}
void print_vec(const std::vector<int> &arr){
  printf("length:%d{\n", arr.size());
  for(auto &n:arr){
    printf("%d,\n", n);
  }
  printf("}\n");
}
int test_pred(){
  std::vector<int> arr{1,2,3,4,5,6,7,8,9,10}; //array for test
  const char str[] = "call add_b"; // a local variance for test
  // one way to define predicate
  auto add_b = xh::predicate([&str](std::vector<int> vec, int b)->std::vector<int>{
    for(auto &num:vec)
      num+=b;
    printf("%s\n", str);
    return vec;
  });
  //another way to define predicate
  auto onlyOdd = PRED(only_odd);
  //something like C++20
  arr | onlyOdd | add_b(10) | PRED(print_vec);
  return 0;
}
int main(){
  test_pred();
  return 0;
}
