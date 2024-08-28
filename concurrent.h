#ifndef _XH_CONCURRENT_H_
#define _XH_CONCURRENT_H_

#include <optional>
#include <atomic>

using namespace std;

namespace xh {

using T = int;

class concurrent_queue {
  struct node {
    T *data;
    atomic<node *> next;
    node(T *val = nullptr) : data(val), next(nullptr) {}
    ~node() { delete data; }
  };
  atomic<node *> head, tail;
  atomic<size_t> _size;

  void _push(node *new_node) {
    node *old_tail = tail.load();
    while (!tail.compare_exchange_weak(old_tail, new_node));
    old_tail->next.store(new_node);
    _size.fetch_add(1);
  }

 public:
  concurrent_queue() : head(new node), tail(head.load()), _size(0) {}
  ~concurrent_queue() {}

  void push(const T &item) { _push(new node(new T(item))); }
  void push(T &&item) { _push(new node(new T(move(item)))); }
  template<class... Args>
  void emplace(Args &&... args) {
    _push(new node(new T(forward<Args>(args)...)));
  }

  optional<T> try_pop() {
    atomic<node *> &next = head.load()->next;
    node *old_next = next.load();
    if (!old_next) return nullopt;
    node *new_next = old_next->next.load();
    while (!next.compare_exchange_weak(old_next, new_next)) {
      if (!old_next) return nullopt;
      new_next = old_next->next.load();
    }
    optional<T> res(*old_next->data);
    delete old_next;
    _size.fetch_sub(1);
    return res;
  }

  T wait_pop() {
    optional<T> res;
    while (!(res = try_pop()).has_value());
    return res.value();
  }

  bool empty() const { return head.load() == tail.load(); }

  size_t size() const { return _size.load(); }

  void clear() {}

};

};

#endif //_XH_CONCURRENT_H_
