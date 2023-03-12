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
//   iterator emplace_hint(const_iterator position, Args&&... args);

#include <algorithm>
#include <cassert>
#include <concepts>
#include <deque>
#include <flat_set>
#include <functional>
#include <vector>

#include "test_macros.h"
#include "../../Emplaceable.h"
#include "min_allocator.h"

int main(int, char**)
{
  {
    using M = std::flat_multiset<Emplaceable>;
    M m;
    std::same_as<M::iterator> auto r = m.emplace_hint(m.cend());
    assert(r == m.begin());
    assert(m.size() == 1);
    assert(*m.begin() == Emplaceable());
    r = m.emplace_hint(m.cend(), 2, 3.5);
    assert(r == m.begin() + 1);
    assert(m.size() == 2);
    assert(*r == Emplaceable(2, 3.5));
    r = m.emplace_hint(m.cbegin(), 2, 3.5);
    assert(r == m.begin() + 1);
    assert(m.size() == 3);
    assert(*r == Emplaceable(2, 3.5));
  }
  {
    using M = std::flat_multiset<int>;
    using R = M::iterator;
    M m;
    std::same_as<R> auto r = m.emplace_hint(m.cend(), 2);
    assert(r == m.begin());
    assert((m == M{2}));
  }
  {
    using M = std::flat_multiset<int, std::greater<int>, std::deque<int, min_allocator<int>>>;
    using R = M::iterator;
    M m = {4, 3, 1};
    std::same_as<R> auto r = m.emplace_hint(m.cend(), 2);
    assert(r == m.begin() + 2);
    assert((m == M{4, 3, 2, 1}));
  }
  {
    // Make sure the insertion point is always closest to the hint.
    struct ByTensDigit {
      bool operator()(int a, int b) const { return a/10 < b/10; }
    };
    using M = std::flat_multiset<int, ByTensDigit>;
    M m;
    M::iterator it;
    it = m.emplace_hint(m.end(), 20); // hint is correct, at begin and end
    assert((m == M{20}));
    assert(it == m.begin());
    it = m.emplace_hint(m.end(), 21); // hint is correct, at end
    assert(std::ranges::equal(m, std::vector{20, 21}));
    assert(it == m.begin() + 1);
    it = m.emplace_hint(m.begin(), 22); // hint is correct, at begin
    assert(std::ranges::equal(m, std::vector{22, 20, 21}));
    assert(it == m.begin());

    it = m.emplace_hint(m.begin(), 10); // hint is correct, at begin
    assert(std::ranges::equal(m, std::vector{10, 22, 20, 21}));
    assert(it == m.begin());
    it = m.emplace_hint(m.begin() + 1, 11); // hint is correct, at upper bound
    assert(std::ranges::equal(m, std::vector{10, 11, 22, 20, 21}));
    assert(it == m.begin() + 1);
    it = m.emplace_hint(m.begin(), 12); // hint is correct, at lower bound
    assert(std::ranges::equal(m, std::vector{12, 10, 11, 22, 20, 21}));
    assert(it == m.begin());

    it = m.emplace_hint(m.end(), 30); // hint is correct, at end
    assert(std::ranges::equal(m, std::vector{12, 10, 11, 22, 20, 21, 30}));
    assert(it == m.begin() + 6);
    it = m.emplace_hint(m.end() - 1, 31); // hint is correct, at lower bound
    assert(std::ranges::equal(m, std::vector{12, 10, 11, 22, 20, 21, 31, 30}));
    assert(it == m.begin() + 6);
    it = m.emplace_hint(m.end(), 32); // hint is correct, at upper bound
    assert(std::ranges::equal(m, std::vector{12, 10, 11, 22, 20, 21, 31, 30, 32}));
    assert(it == m.begin() + 8);

    it = m.emplace_hint(m.begin(), 23); // hint is wrong, at begin
    assert(std::ranges::equal(m, std::vector{12, 10, 11, 23, 22, 20, 21, 31, 30, 32}));
    assert(it == m.begin() + 3);
    it = m.emplace_hint(m.end(), 24); // hint is wrong, at end
    assert(std::ranges::equal(m, std::vector{12, 10, 11, 23, 22, 20, 21, 24, 31, 30, 32}));
    assert(it == m.begin() + 7);
    it = m.emplace_hint(m.begin() + 2, 25); // hint is wrong, toward begin
    assert(std::ranges::equal(m, std::vector{12, 10, 11, 25, 23, 22, 20, 21, 24, 31, 30, 32}));
    assert(it == m.begin() + 3);
    it = m.emplace_hint(m.end() - 2, 26); // hint is wrong, toward end
    assert(std::ranges::equal(m, std::vector{12, 10, 11, 25, 23, 22, 20, 21, 24, 26, 31, 30, 32}));
    assert(it == m.begin() + 9);

    it = m.emplace_hint(m.begin() + 5, 27); // hint is correct, in middle
    assert(std::ranges::equal(m, std::vector{12, 10, 11, 25, 23, 27, 22, 20, 21, 24, 26, 31, 30, 32}));
    assert(it == m.begin() + 5);
  }
  return 0;
}
