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

#include <cassert>
#include <concepts>
#include <deque>
#include <flat_set>
#include <functional>

#include "test_macros.h"
#include "../../Emplaceable.h"
#include "min_allocator.h"

int main(int, char**)
{
  {
    using M = std::flat_set<Emplaceable>;
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
    assert(m.size() == 2);
    assert(*r == Emplaceable(2, 3.5));
  }
  {
    using M = std::flat_set<int>;
    using R = M::iterator;
    M m;
    std::same_as<R> auto r = m.emplace_hint(m.cend(), 2);
    assert(r == m.begin());
    assert((m == M{2}));
  }
  {
    using M = std::flat_set<int, std::greater<int>, std::deque<int, min_allocator<int>>>;
    using R = M::iterator;
    M m = {4, 3, 1};
    std::same_as<R> auto r = m.emplace_hint(m.cend(), 2);
    assert(r == m.begin() + 2);
    assert((m == M{4, 3, 2, 1}));
  }
  return 0;
}
