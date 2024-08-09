#ifndef HZWXLIB_HSTD_HPREDICATE_H
#define HZWXLIB_HSTD_HPREDICATE_H
#include <tuple>
#include "./function.h"
#include <functional>
namespace xh{
template<class F>
inline auto stdFunc(F func){return std::function<function_traits_t<F>>(func);}

/**
 * @brief 谓词Caller(C++20管道运算符)
 * @tparam Func 原函数的类型
 * @tparam Args 函数除第一个参数以外的剩余参数类型
*/
template<class Func, class ...Args>
class predicate_no_call{
private:
    Func __func;
    std::tuple<Args...> __args;
public:
    using subject_type = typename function_traits<Func>::argument_type<0>;
    predicate_no_call(Func func, std::tuple<Args...> &&args)
        :__func(func), __args(std::move(args)){}
    predicate_no_call(const predicate_no_call &)=delete;
    predicate_no_call(predicate_no_call &&)=default;
    predicate_no_call &operator=(const predicate_no_call &)=delete;
    predicate_no_call &operator=(predicate_no_call &&)=default;
    /**
     * @brief 谓词调用(有参版)
     * @param subject 主语（函数第一个函数）
     * @param pred 谓语，也就是TPredicateWrapper对象
     * @return 返回正常函数返回值；即把filter(array,isInt)变成array | filter(isInt)的形式
     * @attention 主语是C数组、参数是指针时，要么数组写成arr+0的形式，要么参数指针写成const
     * @attention 不得更改“主语”，相当于是“主语”的const成员函数
    */
    friend function_return_t<Func> operator|(
        subject_type subject, 
        const predicate_no_call<Func, Args...> &pred
    ){
        auto args=std::tuple_cat(std::make_tuple(subject), pred.__args);
        return std::apply(pred.__func, args);
    }
};
template<class Func, class ...Args>
predicate_no_call(Func, std::tuple<Args...>) -> predicate_no_call<std::function<function_traits_t<Func>>, Args...>;
/**
 * @brief 谓词Wrapper(C++20管道运算符)
 * @tparam TFunc 函数类型
 */
template<class Func>
class predicate
{
private:
    Func __func;
public:
    using subject_type = typename function_traits<Func>::argument_type<0>;
    /**
     * @brief 构造函数，把一个函数打包成“谓词”形式
     * @param func 被打包函数指针
     * @attention 不得更改“主语”，相当于是“主语”的const成员函数
    */
    predicate(Func func):__func(func){}

    predicate(const predicate &)=delete;
    predicate(predicate &&)=default;
    predicate &operator=(const predicate &)=delete;
    predicate &operator=(predicate &&)=default;
    /**
     * @brief 谓词补充
     * @param args 除了第一个参数以外的其余参数
     * @return 返回Caller
    */
    template<class ...Args>
    predicate_no_call<Func,Args...> operator()(Args... args) const {
        return predicate_no_call<Func, Args...>(__func,std::make_tuple(args...));
    }
    /**
     * @brief 谓词调用(无参版)
     * @param subject 主语（函数第一个函数）
     * @param pred 谓语，也就是TPredicateWrapper对象
     * @return 返回正常函数返回值；即把print(a)变成a|print的形式
     * @attention 主语是C数组、参数是指针时，要么数组写成arr+0的形式，要么参数指针写成const
     * @attention 不得更改“主语”，相当于是“主语”的const成员函数
    */
    friend function_return_t<Func> operator|(
        subject_type subject, 
        const predicate<Func> &pred
    ){
        return pred.__func(subject);
    }
    
};
template<class Func>
predicate(Func) -> predicate<std::function<function_traits_t<Func>>>;

#define PRED xh::predicate
}
#endif