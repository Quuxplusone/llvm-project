//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// void replace(container_type&& cont);

#include <deque>
#include <flat_set>
#include <functional>
#include <utility>
#include <vector>

#include "test_macros.h"
#include "min_allocator.h"
#include "./MinSequenceContainer.h"
#include "Counter.h"

template<class S, class V>
concept HasReplaceFrom = requires {
  std::declval<S>().replace(std::declval<V>());
};

struct ThrowingMoveContainer : MinSequenceContainer<int> {
  using MinSequenceContainer<int>::MinSequenceContainer;
  explicit ThrowingMoveContainer() = default;
  ThrowingMoveContainer& operator=(ThrowingMoveContainer&&) {
    // Put some duplicates in, to simulate a "basic exception guarantee" on the container
    this->insert(this->end(), 1);
    this->insert(this->end(), 1);
    this->insert(this->end(), 1);
    throw 42;
  }
};

struct CopyOnlyContainer : MinSequenceContainer<int> {
  using MinSequenceContainer<int>::MinSequenceContainer;
  explicit CopyOnlyContainer() = default;
  CopyOnlyContainer(const CopyOnlyContainer& rhs) { *this = rhs; }
  CopyOnlyContainer& operator=(const CopyOnlyContainer&) = default;
};

int main(int, char**)
{
  {
    using V = std::vector<int>;
    using M = std::flat_set<int>;
    M m = {1, 5, 4, 9, 2};
    V v = {2, 3, 5}; // "Preconditions: The elements of cont are sorted with respect to compare, and cont contains no equal elements."
    // .replace(v) requires an rvalue container as input (sadly)
    static_assert(!HasReplaceFrom<M&, V&>);
    static_assert(!HasReplaceFrom<M&, const V&>);
    static_assert(HasReplaceFrom<M&, V&&>);
    static_assert(!HasReplaceFrom<M&, const V&&>);
    static_assert(!HasReplaceFrom<M&&, V&>);
    static_assert(!HasReplaceFrom<M&&, const V&>);
    static_assert(HasReplaceFrom<M&&, V&&>);
    static_assert(!HasReplaceFrom<M&&, const V&&>);
    ASSERT_SAME_TYPE(decltype(m.replace(std::move(v))), void);
    m.replace(std::move(v));
    assert(v.empty());  // because it's been moved-from
    assert((m == M{2, 3, 5}));
  }
  {
    using V = std::deque<int, min_allocator<int>>;
    using M = std::flat_set<int, std::function<bool(int,int)>, V>;
    M m({1, 3, 6, 10}, std::greater<int>());
    V v = {4, 3}; // "Preconditions: The elements of cont are sorted with respect to compare, and cont contains no equal elements."
    ASSERT_SAME_TYPE(decltype(m.replace(std::move(v))), void);
    m.replace(std::move(v));
    assert(v.empty());  // because it's been moved-from
    assert(m.key_comp()(2, 1) == true); // the comparator hasn't been changed
    assert(m == M({4, 3}, std::greater<int>()));
  }
  {
    // Replacing moves (not copies) the container
    using M = std::flat_set<Counter<int>>;
    M::container_type v = {3, 4, 5};
    auto old = Counter_base::gConstructed;
    M m = {1, 2, 3, 4, 5, 6}; // construct some more Counters
    m.replace(std::move(v));  // but then destroy them here
    assert(Counter_base::gConstructed == old);
    assert(v.empty());  // because it's been moved-from
  }
#ifndef TEST_HAS_NO_EXCEPTIONS
  {
    // The set invariant is restored, even if the function exits via an exception.
    std::flat_set<int, std::less<>, ThrowingMoveContainer> m = {1, 2, 3, 4};
    ThrowingMoveContainer v = {1, 2, 3};
    try {
      m.replace(std::move(v));
      assert(false);
    } catch (int ex) {
      assert(ex == 42);
    }
    assert(m.empty());
  }
#endif
  return 0;
}
