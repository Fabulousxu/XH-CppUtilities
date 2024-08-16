#ifndef _XH_CPP_TOOLS_H_
#define _XH_CPP_TOOLS_H_

#include <iostream>
#include "function_traits.h"

using namespace std;

namespace xh {

// enum name

template<auto>
constexpr auto enum_name() {
#if __GNUC__ || __clang__
	string_view name = __PRETTY_FUNCTION__;
	size_t start = name.find('=') + 2, end = name.size() - 1;
#elif _MSC_VER
	string_view name = __FUNCSIG__;
	size_t start = name.find('<') + 1, end = name.rfind('>');
#endif
	name = string_view{name.data() + start, end - start};
	if ((start = name.rfind("::")) != string_view::npos)
		name = string_view{name.data() + start + 2, name.size() - start - 2};
	return name.find(')') == string_view::npos ? name : "";
}

template<typename T, size_t N = 0>
constexpr auto enum_max() {
	if constexpr (!enum_name<static_cast<T>(N)>().empty())
		return enum_max<T, N + 1>();
	return N;
}

template<typename T>
requires is_enum_v<T>
constexpr auto enum_name(T value) {
	constexpr auto num = enum_max<T>();
	constexpr auto names = []<size_t... N>(std::index_sequence<N...>) {
		return std::array<std::string_view, num>{
			enum_name<static_cast<T>(N)>()...};
	}(std::make_index_sequence<num>{});
	return names[static_cast<size_t>(value)];
}


// cpp tools

#define USE_STD_IO using std::cin, std::cout, std::cerr, std::endl;

/**
 * @brief 接口宏，继承接口时须把子类名称填入模板参数中\
 * @brief 建议只是使用这套标准，而不是直接用这个宏。这样就可以声明带模板的接口了
 * @param InterfaceName 接口类名称
 * @attention 接口中使用 @c Derived 作为继承该接口的类的代词；接口宏是由模板实现的，故不接受模板
*/
#define H_INTERFACE(InterfaceName) template<class Derived> class InterfaceName

template<class ...BaseTs>
class DerivedBaseTs : public BaseTs ... {};
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
H_INTERFACE(TComparable)
{
 public:
	/**
	 * @brief 比较函数(需要继承实现)
	 * @param obj 另一个对象
	 * @return >0为大于, =0为等于, <0为小于
	*/
	virtual int compare(const Derived &obj) const = 0;
	virtual bool operator<(const Derived &obj) const { return compare(obj) < 0; }
	virtual bool operator<=(const Derived &obj) const {
		return compare(obj) <= 0;
	}
	virtual bool operator==(const Derived &obj) const { return !compare(obj); }
	virtual bool operator!=(const Derived &obj) const { return compare(obj); }
	virtual bool operator>=(const Derived &obj) const {
		return compare(obj) >= 0;
	}
	virtual bool operator>(const Derived &obj) const { return compare(obj) > 0; }
};

}
#endif //_XH_CPP_TOOLS_H_
