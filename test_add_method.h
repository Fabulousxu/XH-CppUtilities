#define ADD_METHOD(ptrVar, ...) {\
  struct H{\
    using dest = decltype(ptrVar);\
    xh::function_return_t<dest> func __VA_ARGS__\
    static dest ptr(){return (dest)(&H::func);}\
  };\
  ptrVar = H::ptr();\
}
