//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// template <class... Args>
//   iterator emplace(Args&&... args);

#include <algorithm>
#include <cassert>
#include <concepts>
#include <deque>
#include <flat_set>
#include <functional>
#include <utility>

#include "test_macros.h"
#include "../../Emplaceable.h"
#include "min_allocator.h"

int main(int, char**)
{
  {
    using M = std::flat_multiset<Emplaceable>;
    using R = M::iterator;
    M m;
    std::same_as<R> auto r = m.emplace();
    assert(r == m.begin());
    assert(*r == Emplaceable());
    assert(m.size() == 1);
    r = m.emplace(2, 3.5);
    assert(r == m.begin() + 1);
    assert(*r == Emplaceable(2, 3.5));
    assert(m.size() == 2);
    r = m.emplace(2, 3.5);
    assert(r == m.begin() + 2); // insert at upper_bound
    assert(*r == Emplaceable(2, 3.5));
    assert(m.size() == 3);
  }
  {
    using M = std::flat_multiset<int>;
    using R = M::iterator;
    M m = {1, 3, 4};
    std::same_as<R> auto r = m.emplace(2);
    assert(r == m.begin() + 1);
    assert(*r == 2);
    assert((m == M{1, 2, 3, 4}));
    r = m.emplace(5);
    assert(r == m.begin() + 4);
    assert(*r == 5);
    assert((m == M{1, 2, 3, 4, 5}));
    r = m.emplace(2);
    assert(r == m.begin() + 2);
    assert(*r == 2);
    assert((m == M{1, 2, 2, 3, 4, 5}));
    r = m.emplace(); // value-initialize int(0)
    assert(r == m.begin());
    assert(*r == 0);
    assert((m == M{0, 1, 2, 2, 3, 4, 5}));
  }
  {
    using M = std::flat_multiset<int, std::greater<int>, std::deque<int, min_allocator<int>>>;
    using R = M::iterator;
    M m = {1, 3, 4};
    std::same_as<R> auto r = m.emplace(2);
    assert(r == m.begin() + 2);
    assert(*r == 2);
    assert((m == M{1, 2, 3, 4}));
    r = m.emplace(5);
    assert(r == m.begin());
    assert(*r == 5);
    assert((m == M{1, 2, 3, 4, 5}));
    r = m.emplace(2);
    assert(r == m.begin() + 4);
    assert(*r == 2);
    assert((m == M{1, 2, 2, 3, 4, 5}));
    r = m.emplace(); // value-initialize int(0)
    assert(r == m.begin() + 6);
    assert(*r == 0);
    assert((m == M{0, 1, 2, 2, 3, 4, 5}));
  }
  {
    // Make sure the insertion point is always upper_bound.
    struct ByTensDigit {
      bool operator()(int a, int b) const { return a/10 < b/10; }
    };
    using M = std::flat_multiset<int, ByTensDigit>;
    M m;
    m.emplace(20);
    assert((m == M{20}));
    m.emplace(25);
    assert(std::ranges::equal(m, std::vector{20, 25}));
    m.emplace(22);
    assert(std::ranges::equal(m, std::vector{20, 25, 22}));
    m.emplace(10);
    assert(std::ranges::equal(m, std::vector{10, 20, 25, 22}));
    m.emplace(15);
    assert(std::ranges::equal(m, std::vector{10, 15, 20, 25, 22}));
    m.emplace(12);
    assert(std::ranges::equal(m, std::vector{10, 15, 12, 20, 25, 22}));
    m.emplace(30);
    assert(std::ranges::equal(m, std::vector{10, 15, 12, 20, 25, 22, 30}));
    m.emplace(21);
    assert(std::ranges::equal(m, std::vector{10, 15, 12, 20, 25, 22, 21, 30}));
    m.emplace(31);
    assert(std::ranges::equal(m, std::vector{10, 15, 12, 20, 25, 22, 21, 30, 31}));
  }
  return 0;
}
