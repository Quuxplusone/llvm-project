//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// size_type erase(const key_type& k);

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
    using M = std::flat_multiset<int>;
    M m = {1,1,2,3,4,5,5,6,6,7,8};
    ASSERT_SAME_TYPE(decltype(m.erase(9)), M::size_type);
    auto n = m.erase(9);
    assert(n == 0);
    assert((m == M{1,1,2,3,4,5,5,6,6,7,8}));
    n = m.erase(4);
    assert(n == 1);
    assert((m == M{1,1,2,3,5,5,6,6,7,8}));
    n = m.erase(1);
    assert(n == 2);
    assert((m == M{2,3,5,5,6,6,7,8}));
    n = m.erase(8);
    assert(n == 1);
    assert((m == M{2,3,5,5,6,6,7}));
    n = m.erase(3);
    assert(n == 1);
    assert((m == M{2,5,5,6,6,7}));
    n = m.erase(4);
    assert(n == 0);
    assert((m == M{2,5,5,6,6,7}));
    n = m.erase(6);
    assert(n == 2);
    assert((m == M{2,5,5,7}));
    n = m.erase(7);
    assert(n == 1);
    assert((m == M{2,5,5}));
    n = m.erase(2);
    assert(n == 1);
    assert((m == M{5,5}));
    n = m.erase(5);
    assert(n == 2);
    assert(m.empty());
  }
  {
    using M = std::flat_multiset<int, std::greater<>, std::deque<int, min_allocator<int>>>;
    M::container_type container = {5,5,6,6,7,8};
    container.insert(container.begin(), {1,1,2,3,4});
    M m = M(std::move(container));
    ASSERT_SAME_TYPE(decltype(m.erase(9)), M::size_type);
    auto n = m.erase(9);
    assert(n == 0);
    assert((m == M{1,1,2,3,4,5,5,6,6,7,8}));
    n = m.erase(4);
    assert(n == 1);
    assert((m == M{1,1,2,3,5,5,6,6,7,8}));
    n = m.erase(1);
    assert(n == 2);
    assert((m == M{2,3,5,5,6,6,7,8}));
    n = m.erase(8);
    assert(n == 1);
    assert((m == M{2,3,5,5,6,6,7}));
    n = m.erase(3);
    assert(n == 1);
    assert((m == M{2,5,5,6,6,7}));
    n = m.erase(4);
    assert(n == 0);
    assert((m == M{2,5,5,6,6,7}));
    n = m.erase(6);
    assert(n == 2);
    assert((m == M{2,5,5,7}));
    n = m.erase(7);
    assert(n == 1);
    assert((m == M{2,5,5}));
    n = m.erase(2);
    assert(n == 1);
    assert((m == M{5,5}));
    n = m.erase(5);
    assert(n == 2);
    assert(m.empty());
  }
  return 0;
}
