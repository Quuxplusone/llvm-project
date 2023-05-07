//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// template<container-compatible-range<value_type> R>
//   void insert_range(R&& rg);

#include <algorithm>
#include <deque>
#include <flat_set>
#include <functional>
#include <ranges>
#include <vector>

#include "MoveOnly.h"
#include "test_macros.h"
#include "test_iterators.h"
#include "min_allocator.h"

int main(int, char**)
{
  {
    using M = std::flat_set<int>;
    using It = forward_iterator<const int*>;
    M m = {10,8,5,2,1};
    int ar[] = {3,1,4,1,5,9,5,8};
    std::ranges::subrange r = { It(ar), It(ar + 8) };
    static_assert(std::ranges::common_range<decltype(r)>);
    m.insert_range(r);
    assert((m == M{1,2,3,4,5,8,9,10}));
  }
  {
    using M = std::flat_set<int, std::greater<>, std::deque<int, min_allocator<int>>>;
    using It = cpp20_input_iterator<const int*>;
    M m = {8,5,3,2};
    int ar[] = {3,1,4,1,5,9,5,8};
    std::ranges::subrange r = { It(ar), sentinel_wrapper<It>(It(ar + 8)) };
    static_assert(!std::ranges::common_range<decltype(r)>);
    m.insert_range(r);
    assert((m == M{9,8,5,4,3,2,1}));
  }
  {
    // The "uniquing" part uses the comparator, not operator==.
    struct ModTen {
      bool operator()(int a, int b) const { return (a % 10) < (b % 10); }
    };
    using M = std::flat_set<int, ModTen>;
    M m = {21, 43, 15, 37};
    int ar[] = {33, 18, 55, 18, 42};
    m.insert_range(ar);
    assert((m == M{21, 42, 43, 15, 37, 18}));
  }
  {
    // The "uniquing" part uses the comparator, not operator==.
    struct ModTen {
      bool operator()(int a, int b) const { return (a % 10) < (b % 10); }
    };
    using M = std::flat_set<int, ModTen>;
    M m = {21, 43, 15, 37};
    int ar[] = {33, 18, 55, 28, 42};
    m.insert_range(ar);
    LIBCPP_ASSERT((m == M{21, 42, 43, 15, 37, 18}));
  }
  {
    // Items are forwarded correctly from the input range (P2767).
    MoveOnly a[] = { 3, 1, 4, 1, 5 };
    std::flat_set<MoveOnly> m;
    m.insert_range(a | std::views::as_rvalue);
    MoveOnly expected[] = {1, 3, 4, 5};
    assert(std::ranges::equal(m, expected));
  }
  return 0;
}
