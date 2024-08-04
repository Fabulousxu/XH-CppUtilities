#ifndef HZWX_MEMFUNC_STD_H
#define HZWX_MEMFUNC_STD_H
namespace hstd
{
    /**
     * @brief 通过重载调用符号，把成员函数转为普通“函数”的类
     * @tparam T 类名
     * @tparam _Ret 成员函数返回类型
     * @tparam Args 成员函数参数列表
     */
    template <class T, typename _Ret, typename... Args>
    class TMemFunc
    {
    public:
        /**
         * @brief 构造函数
         * @param pMemFunc 成员函数指针
         */
        TMemFunc(_Ret (T::*pMemFunc)(Args...)) : __pMemFunc(pMemFunc) {}
        /**
         * @brief 调用操作符
         * @param rObj 对象引用
         * @param args 参数列表
         * @attention 是通过引用调用
         */
        _Ret operator()(T &rObj, Args... args)
        {
            return (rObj.*__pMemFunc)(args...);
        }
        /**
         * @brief 调用操作符
         * @param pObj 对象指针
         * @param args 参数列表
         * @attention 是通过指针调用
         */
        _Ret operator()(T *pObj, Args... args) { return (pObj->*__pMemFunc)(args...); }

    private:
        _Ret (T::*const __pMemFunc)(Args...);
    };
    /**
     * @brief 通过重载调用符号，把成员函数(const)转为普通“函数”的类
     * @tparam T 类名
     * @tparam _Ret 成员函数返回类型
     * @tparam Args 成员函数参数列表
     */
    template <class T, typename _Ret, typename... Args>
    class TMemFuncC
    {
    public:
        /**
         * @brief 构造函数
         * @param pMemFunc 成员函数指针
         */
        TMemFuncC(_Ret (T::*pMemFuncC)(Args...) const)
            : __pMemFuncC(pMemFuncC) {}
        _Ret operator()(const T &rObj, Args... args) { return (rObj.*__pMemFuncC)(args...); }
        /**
         * @brief 调用操作符
         * @param pObj 对象指针
         * @param args 参数列表
         * @attention 是通过指针调用
         */
        _Ret operator()(const T *pObj, Args... args) { return (pObj->*__pMemFuncC)(args...); }

    private:
        _Ret (T::*const __pMemFuncC)(Args...) const;
    };
    /**
     * @brief 把成员函数转为普通“函数”
     * @tparam T 类名
     * @tparam _Ret 成员函数返回类型
     * @tparam Args 参数列表
     * @param pf 被转换的成员函数指针
     * @return 一个TMemFunc对象，由于重载了调用符号所以可以当函数用
     */
    template <class T, typename _Ret, typename... Args>
    TMemFunc<T, _Ret, Args...> memFunc(_Ret (T::*pf)(Args...))
    {
        return TMemFunc<T, _Ret, Args...>(pf);
    }
    /**
     * @brief 把成员函数(const)转为普通“函数”
     * @tparam T 类名
     * @tparam _Ret 成员函数返回类型
     * @tparam Args 参数列表
     * @param pf 被转换的成员函数(const)指针
     * @return 一个TMemFuncC对象，由于重载了调用符号所以可以当函数用
     */
    template <class T, typename _Ret, typename... Args>
    TMemFuncC<T, _Ret, Args...> memFunc(_Ret (T::*pf)(Args...) const)
    {
        return TMemFuncC<T, _Ret, Args...>(pf);
    }
}

#endif