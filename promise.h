#ifndef XH_PROMISE_H
#define XH_PROMISE_H

#include <functional>
#include "function_traits.h"

namespace xh {
using std::function;

template<typename>
class Promise;

template<typename _Ret, typename... _Args>
class Promise<_Ret(_Args...)> {
	function<_Ret(_Args...)> _promise;
 public:
	template<typename _F>
	Promise(_F &&f) noexcept: _promise(std::forward<_F>(f)) {}
	_Ret operator()(_Args &&...args) const {
		return _promise(std::forward<_Args>(args)...);
	}
	template<typename _F>
	Promise<std::invoke_result_t<_F, _Ret>(_Args...)> then(_F &&f) {
		return {[g = std::forward(f), p = std::move(_promise)](_Args &&...args) {
			return g(p(std::forward<_Args>(args)...));
		}};
	}
};

template<typename _T>
Promise(_T) -> Promise<function_traits_t<_T>>;

}; // namespace xh

#endif //XH_PROMISE_H
