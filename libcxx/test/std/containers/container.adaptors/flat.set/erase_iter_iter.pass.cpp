//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// iterator erase(const_iterator first, const_iterator last);

#include <compare>
#include <concepts>
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
    ASSERT_SAME_TYPE(decltype(m.erase(m.cend(), m.end())), M::iterator);
    M::iterator it = m.erase(m.cbegin() + 5, m.cbegin() + 5);
    assert(it == m.cbegin() + 5);
    assert((m == M{1,2,3,4,5,6,7,8}));
    it = m.erase(m.cbegin() + 3, m.begin() + 4);
    assert(it == m.begin() + 3);
    assert((m == M{1,2,3,5,6,7,8}));
    it = m.erase(m.begin() + 2, m.cbegin() + 5);
    assert(it == m.begin() + 2);
    assert((m == M{1,2,7,8}));
    it = m.erase(m.begin(), it);
    assert(it == m.begin());
    assert((m == M{7,8}));
    it = m.erase(m.begin(), m.end());
    assert(it == m.end());
    assert(m.empty());
  }
  {
    using M = std::flat_set<int, std::less<>, std::deque<int, min_allocator<int>>>;
    M::container_type container = {5,6,7,8};
    container.insert(container.begin(), {1,2,3,4});
    M m = M(std::move(container));
    ASSERT_SAME_TYPE(decltype(m.erase(m.cend(), m.end())), M::iterator);
    M::iterator it = m.erase(m.cbegin() + 5, m.cbegin() + 5);
    assert(it == m.cbegin() + 5);
    assert((m == M{1,2,3,4,5,6,7,8}));
    it = m.erase(m.cbegin() + 3, m.begin() + 4);
    assert(it == m.begin() + 3);
    assert((m == M{1,2,3,5,6,7,8}));
    it = m.erase(m.begin() + 2, m.cbegin() + 5);
    assert(it == m.begin() + 2);
    assert((m == M{1,2,7,8}));
    it = m.erase(m.begin(), it);
    assert(it == m.begin());
    assert((m == M{7,8}));
    it = m.erase(m.begin(), m.end());
    assert(it == m.end());
    assert(m.empty());
  }
  return 0;
}
