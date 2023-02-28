//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// value_type displace(const_iterator position);

#include <deque>
#include <flat_set>
#include <functional>
#include <utility>

#include "test_macros.h"
#include "min_allocator.h"

int main(int, char**)
{
  {
    using M = std::flat_set<int>;
    M m = {1,2,3,4,5,6,7,8};
    assert(m.size() == 8);
    ASSERT_SAME_TYPE(decltype(m.displace(m.begin())), int);
    ASSERT_SAME_TYPE(decltype(m.displace(m.cbegin())), int);
    int v = m.displace(m.cbegin() + 3);
    assert(v == 4);
    assert((m == M{1,2,3,5,6,7,8}));

    v = m.displace(m.cbegin());
    assert(v == 1);
    assert((m == M{2,3,5,6,7,8}));

    v = m.displace(m.cbegin() + 5);
    assert(v == 8);
    assert((m == M{2,3,5,6,7}));

    v = m.displace(m.cbegin() + 1);
    assert(v == 3);
    assert((m == M{2,5,6,7}));
  }
  {
    using M = std::flat_set<int, std::less<int>, std::deque<int, min_allocator<int>>>;
    std::deque<int, min_allocator<int>> container = {3,4};
    container.insert(container.begin(), {1,2}); // now container is [_ 1 2] [3 4 _]
    M m = M(std::sorted_unique, std::move(container));
    ASSERT_SAME_TYPE(decltype(m.displace(m.begin())), int);
    ASSERT_SAME_TYPE(decltype(m.displace(m.cbegin())), int);
    int v = m.erase(m.cbegin() + 1); // shift right: m is [_ _ 1] [3 4 _]
    assert(v == 2);
    assert((m == M{1,3,4}));
    v = m.erase(m.cbegin() + 1); // shift left: m is [_ _ 1] [4 _ _]
    assert(v == 3);
    assert((m == M{1,4}));
    v = m.erase(m.cbegin());
    assert(v == 1);
    assert(m == M{4});
    v = m.erase(m.cbegin());
    assert(it == 4);
    assert(m.empty());
  }
  {
    // Verify that it works with move-only types (its raison d'etre).
    using M = std::flat_set<MoveOnly>;
    M m;
    for (int i = 1; i <= 8; ++i) {
      m.insert(i);
    }
    assert(m.size() == 8);
    ASSERT_SAME_TYPE(decltype(m.displace(m.begin())), MoveOnly);
    ASSERT_SAME_TYPE(decltype(m.displace(m.cbegin())), MoveOnly);
    MoveOnly v = m.displace(m.cbegin() + 3);
    assert(v == 4);

    v = m.displace(m.cbegin());
    assert(v == 1);

    v = m.displace(m.cbegin() + 5);
    assert(v == 8);

    v = m.displace(m.cbegin() + 1);
    assert(v == 3);
    assert(m.size() == 4);
  }
  return 0;
}
