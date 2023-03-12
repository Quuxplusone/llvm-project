//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <set>

// value_type displace(const_iterator position);

#include <functional>
#include <set>
#include <utility>

#include "test_macros.h"
#include "min_allocator.h"
#include "MoveOnly.h"

template<class It>
It Next(It it, int n) {
  std::advance(it, n);
  return it;
}

int main(int, char**)
{
  {
    using M = std::multiset<int>;
    const int a[] = {1,3,3,4,5,5,8,8};
    M m(a, a+8);
    assert(m.size() == 8);
    ASSERT_SAME_TYPE(decltype(m.displace(m.begin())), int);
    ASSERT_SAME_TYPE(decltype(m.displace(m.cbegin())), int);

    int v = m.displace(Next(m.cbegin(), 3));
    assert(v == 4);
    assert(m.size() == 7);
#if TEST_STD_VER >= 11
    assert((m == M{1,3,3,5,5,8,8}));
#endif

    v = m.displace(m.cbegin());
    assert(v == 1);
    assert(m.size() == 6);

    v = m.displace(Next(m.cbegin(), 5));
    assert(v == 8);
    assert(m.size() == 5);

    v = m.displace(Next(m.cbegin(), 1));
    assert(v == 3);
    assert(m.size() == 4);
#if TEST_STD_VER >= 11
    assert((m == M{3,5,5,8}));
#endif
  }
  {
    using M = std::multiset<int, std::less<int>, min_allocator<int> >;
    const int a[] = {1,2,3,4};
    M m(a, a+4);
    ASSERT_SAME_TYPE(decltype(m.displace(m.begin())), int);
    ASSERT_SAME_TYPE(decltype(m.displace(m.cbegin())), int);
    int v = m.displace(Next(m.cbegin(), 1));
    assert(v == 2);
    assert(m.size() == 3);
    v = m.displace(Next(m.cbegin(), 1));
    assert(v == 3);
    assert(m.size() == 2);
    v = m.displace(m.cbegin());
    assert(v == 1);
    assert(m.size() == 1);
    v = m.displace(m.cbegin());
    assert(v == 4);
    assert(m.empty());
  }
#if TEST_STD_VER >= 11
  {
    // Verify that it works with move-only types (its raison d'etre).
    using M = std::multiset<MoveOnly>;
    M m;
    for (int i = 1; i <= 8; ++i) {
      m.insert(i);
    }
    assert(m.size() == 8);
    ASSERT_SAME_TYPE(decltype(m.displace(m.begin())), MoveOnly);
    ASSERT_SAME_TYPE(decltype(m.displace(m.cbegin())), MoveOnly);

    MoveOnly v = m.displace(Next(m.cbegin(), 3));
    assert(v == 4);

    v = m.displace(m.cbegin());
    assert(v == 1);

    v = m.displace(Next(m.cbegin(), 5));
    assert(v == 8);

    v = m.displace(Next(m.cbegin(), 1));
    assert(v == 3);
    assert(m.size() == 4);
  }
#endif
  return 0;
}
