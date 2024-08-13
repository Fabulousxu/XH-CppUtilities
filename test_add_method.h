#define ADD_METHOD(ptrVar, ...) {\
  struct H{\
    using dest = decltype(ptrVar);\
    xh::callable_return_t<xh::remove_member_function_class_t<dest>> func __VA_ARGS__\
    static dest ptr(){return (dest)(&H::func);}\
  };\
  ptrVar = H::ptr();\
};

/**
 * @example 1:
 * int (Test::* ptr)(int) = nullptr; //ptr类型必须声明
 * ADD_METHOD(ptr, (int a){
 *     Test *self = (Test*)this;
 *     ...
 * })
 * 
 * @example 2
 * int (Test::* ptr)(int) const = nullptr;
 * ADD_METHOD(ptr, (int a) const {
 *     const Test *self = (const Test*)this;
 *     ...
 * })
 */
