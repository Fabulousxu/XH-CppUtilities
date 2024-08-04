#ifndef HZWXLIB_HSTD_HPREDICATE_H
#define HZWXLIB_HSTD_HPREDICATE_H
#include <tuple>
namespace hstd{

/**
 * @brief 谓词Caller(C++20管道运算符)
 * @tparam TFunc 函数类型
 * @tparam Args 函数剩余参数
*/
template<class TFunc, class ...Args>
class TPredicateCaller{
private:
    TFunc __func;
    std::tuple<Args...> __args;
public:
    TPredicateCaller(TFunc func, std::tuple<Args...> &&args):__func(func), __args(std::move(args)){}
    /**
     * @brief 谓词调用(有参版)
     * @param subject 主语（函数第一个函数）
     * @param pred 谓语，也就是TPredicateWrapper对象
     * @return 返回正常函数返回值；即把filter(array,isInt)变成array | filter(isInt)的形式
     * @attention 主语是C数组、参数是指针时，要么数组写成arr+0的形式，要么参数指针写成const
     * @attention 不得更改“主语”，相当于是“主语”的const成员函数
    */
    template<class T>
    friend auto operator|(const T &subject, const TPredicateCaller<TFunc, Args...> &pred){
        auto args=std::tuple_cat(std::make_tuple(subject), pred.__args);
        return std::apply(pred.__func, args);
    }
};

/**
 * @brief 谓词Wrapper(C++20管道运算符)
 * @tparam TFunc 函数类型
 */
template<class TFunc>
class TPredicateWrapper
{
private:
    TFunc __func;
public:
    /**
     * @brief 构造函数，把一个函数打包成“谓词”形式
     * @param func 被打包函数指针
     * @attention 不得更改“主语”，相当于是“主语”的const成员函数
    */
    TPredicateWrapper(TFunc func):__func(func){}
    /**
     * @brief 谓词补充
     * @param args 除了第一个参数以外的其余参数
     * @return 返回Caller
    */
    template<class ...Args>
    TPredicateCaller<TFunc, Args...> operator()(Args... args) const {
        TPredicateCaller<TFunc, Args...> res(__func,std::make_tuple(args...));
        return res;
    }
    /**
     * @brief 谓词调用(无参版)
     * @param subject 主语（函数第一个函数）
     * @param pred 谓语，也就是TPredicateWrapper对象
     * @return 返回正常函数返回值；即把print(a)变成a|print的形式
     * @attention 主语是C数组、参数是指针时，要么数组写成arr+0的形式，要么参数指针写成const
     * @attention 不得更改“主语”，相当于是“主语”的const成员函数
    */
    template<class T>
    friend auto operator|(const T &subject, const TPredicateWrapper<TFunc> &pred){
        return pred.__func(subject);
    }
};
#define PRED hstd::TPredicateWrapper
}
#endif