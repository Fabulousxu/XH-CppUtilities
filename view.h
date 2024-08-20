#ifndef _XH_PREDICATE_H_
#define _XH_PREDICATE_H_

#include <functional>
#include "function_traits.h"

using namespace std;

namespace xh {

#define ITERATOR_TRAITS_DEF(iter, ptr_iter) \
template<class Container>\
struct iter##_traits{\
    using type = typename Container::iter;\
};\
template<class T>\
struct iter##_traits<T*>{\
    using type = ptr_iter;\
};\
template<class T, size_t N>\
struct iter##_traits<T[N]>:iter##_traits<T*>{};\
template<class T>\
using iter##_traits_t = typename iter##_traits<T>::type;

ITERATOR_TRAITS_DEF(iterator, T *)
ITERATOR_TRAITS_DEF(const_iterator, const T*)

#undef ITERATOR_TRIATS_DEF

template<class Container>
class view {
 public:
  using iterator = typename const_iterator_traits<Container>::type;
  using container = Container;
 protected:
  iterator _begin;
  iterator _end;
 public:
  view(Container &ctn) : _begin(std::begin(ctn)), _end(std::end(ctn)) {}
  view(const iterator &begin_, const iterator &end_)
      : _begin(begin_), _end(end_) {}

  view(const view &v) {
    memcpy(this, &v, sizeof(view<Container>));
  }
  view(view &&v) {
    memcpy(this, &v, sizeof(view<Container>));
  }
  view &operator=(const view &v) {
    memcpy(this, &v, sizeof(view<Container>));
    return *this;
  }
  view &operator=(view &&v) {
    memcpy(this, &v, sizeof(view<Container>));
    return *this;
  }
  iterator begin() const { return _begin; }
  iterator end() const { return _end; }
};

};

#endif //_XH_PREDICATE_H_
