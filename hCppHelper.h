#ifndef HZWXLIB_HSTD_HCPPHELPER_H
#define HZWXLIB_HSTD_HCPPHELPER_H
/*
    @author Hzwx158
    @date 2024.2.15
    本头文件旨在简化（大概）C++写法，让C++可读性加强一些
    含有各种宏和一些类型转化
    
    介绍一下用到的知识：
    C语言部分：
    * #define，调用时直接将后面的完完全全替换进去，可以用##连接名字
    C++部分：
    * using T=...，是C++版本的typedef。typedef后面跟的是一个变量声明，然后“变量名”变成这个类型的“别名”；using则是后面直接跟类型名
    * 例如 typedef int (*pArray3)[3]和using pArray3=int(*)[3]是等价的
    * 
    * template里面的class ...Args：Args是类型的一个聚合“包”，可以理解为对int,char,int...等一堆模板参数的“打包”
    * Args ...args，是声明了一个参数的聚合“包”args，可以理解为是对int arg1, char arg2, int arg3...里arg1,arg2,arg3...的“打包”
    * 上面两条说明，...在前是声明聚合“包”
    * Args...是对Args的“解包”，也就是字面意义上把Args...换成int,char,int...这一堆模板参数
    * args...是对args的“解包”，也就是字面意义上把args...换成arg1,arg2,arg3...这一堆东西(或者说1,2,3,...)
    * 上面两条说明，...在后是对聚合“包”的“解包”，字面意义上把聚合“包”换为“包”里的东西。这个“字面意义上”和#define的“字面意义上替代”意思差不多
    * 值得注意的是，解包是解“...”前面那个含args的表达式。比如func(args...)等价于func(1,2,3)，而func(args)...等价于func(1),func(2),func(3)
    * 
    * lambda表达式：
    * 基本语法：[捕获列表](参数列表)->返回类型{函数体}
    * 捕获列表里是捕获局部变量给lambda用；返回类型、参数列表、函数体和正常函数一样；“->返回类型”部分可以省略
    * 注意，每一个lambda表达式都是一个独特的类型，即便两个lambda长得完全一样，他俩也是俩类型。不可以decltype([](){}) another
*/
#include <iostream>
#include <functional>
#include "./hMemfunc.h"
#include "./hPredicate.h"
namespace hstd{
/**
 * @brief debug输出
 * @param from debug来源
 * @param str 输出内容
*/
inline void debug(const char *from, const char *str)
{
    printf("From:%s\n\t%s",from,str);
}
//使用cin cout endl
#define UseStdIO using std::cin;using std::cout;using std::endl;

//------------------------------函数用------------------------------

/**
 * @brief 只运行一次助手 
 * @param retval 多次运行时的返回值，可以不填
*/
#define H_RUN_ONCE(retval) \
static bool __runOnceHelper=false;\
if(__runOnceHelper) return retval;\
__runOnceHelper=true;

//返回无参默认值(如果不想用auto的话)
struct autoReturn
{
    template<class T>
    operator T(){return T();}
};


//------------------------------类用------------------------------

template<class MemType> 
class _Getter{
    const std::function<MemType()> _getterFunc;
public:
    template<class LambdaF>
    explicit _Getter(LambdaF &&getterFunc):_getterFunc(getterFunc){}
    // _Getter(const _Getter &)=delete;
    // _Getter(_Getter &&)=delete;
    _Getter &operator=(const _Getter &)=delete;
    _Getter &operator=(_Getter &&)=delete;
    operator MemType() const {
        return _getterFunc();
    }
};
#define getter(getterName, Type) const hstd::_Getter<Type> getterName=(hstd::_Getter<Type>)[&]()->Type
template<class ArgType, class RetType>
class _Setter{
    const std::function<RetType(ArgType)> _setterFunc;
public:
    template<class LambdaF>
    explicit _Setter(LambdaF &&setterFunc):_setterFunc(setterFunc){}
    _Setter &operator=(const _Setter &)=delete;
    _Setter &operator=(_Setter &&)=delete;
    RetType operator=(ArgType arg) const{
        return _setterFunc(arg);
    }
};
#define setter(setterName, ArgType, RetType) const hstd::_Setter<ArgType, RetType> setterName=(hstd::_Setter<ArgType, RetType>)[&](ArgType arg)->RetType

/**
 * @brief 接口宏，继承接口时须把子类名称填入模板参数中\
 * @brief 建议只是使用这套标准，而不是直接用这个宏。这样就可以声明带模板的接口了
 * @param InterfaceName 接口类名称
 * @attention 接口中使用 @c Derived 作为继承该接口的类的代词；接口宏是由模板实现的，故不接受模板
*/
#define H_INTERFACE(InterfaceName) template<class Derived> class InterfaceName

template<class ...BaseTs>
class DerivedBaseTs:public BaseTs...{};
/**
 * @brief 单例/匿名模式的接口实现辅助宏
 * @param InterfaceNames 要实现的接口基类
 * @attention 只能用来声明变量，不能直接当作变量
 * @example 用法:
 * H_SINGLE_IMPL(TClickEvent){
 *     void onclick() const override{...}
 *     ...
 * } obj;
*/
#define H_SINGLE_IMPL(...) class final:public hstd::DerivedBaseTs<__VA_ARGS__>

//可输入类标签
#define H_INPUTABLE(ClassName) \
friend std::istream & operator>>(std::istream &ism, ClassName &obj);
//可输出类标签
#define H_OUTPUTABLE(ClassName) \
friend std::ostream & operator<<(std::ostream &osm, const ClassName &obj);
//可输入输出类标签
#define H_IOABLE(ClassName) H_INPUTABLE(ClassName) H_OUTPUTABLE(ClassName)

//有指针的类标签
#define H_HAS_POINTER_MEMBER(ClassName) \
ClassName(const ClassName &obj);\
ClassName(ClassName &&obj);\
~ClassName();\
ClassName &operator=(const ClassName &obj);\
ClassName &operator=(ClassName &&obj);

//可比较类(只需要实现compare函数，<0是小于，=0是等于，>0是大于)
#define H_COMPARABLE(ClassName)\
bool operator<(const ClassName &obj) const {return compare(obj)<0;}\
bool operator<=(const ClassName &obj) const {return compare(obj)<=0;}\
bool operator==(const ClassName &obj) const {return !compare(obj);}\
bool operator!=(const ClassName &obj) const {return compare(obj);}\
bool operator>=(const ClassName &obj) const {return compare(obj)>=0;}\
bool operator>(const ClassName &obj) const {return compare(obj)>0;}\
int compare(const ClassName &obj) const;

//可比较类接口
H_INTERFACE(TComparable){
public:
    /**
     * @brief 比较函数(需要继承实现)
     * @param obj 另一个对象
     * @return >0为大于, =0为等于, <0为小于
    */
    virtual int compare(const Derived &obj) const = 0;
    virtual bool operator<(const Derived &obj) const {return compare(obj)<0;}
    virtual bool operator<=(const Derived &obj) const {return compare(obj)<=0;}
    virtual bool operator==(const Derived &obj) const {return !compare(obj);}
    virtual bool operator!=(const Derived &obj) const {return compare(obj);}
    virtual bool operator>=(const Derived &obj) const {return compare(obj)>=0;}
    virtual bool operator>(const Derived &obj) const {return compare(obj)>0;}
};


} 
#endif
