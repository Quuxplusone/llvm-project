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

#include "test_macros.h"
#include "test_iterators.h"
#include "min_allocator.h"

int main(int, char**)
{
  {
    using M = std::flat_multiset<int>;
    using It = forward_iterator<const int*>;
    M m = {10,8,5,2,1};
    int ar[] = {3,1,4,1,5,9,5,8};
    std::ranges::subrange r = { It(ar), It(ar + 8) };
    static_assert(std::ranges::common_range<decltype(r)>);
    m.insert_range(r);
    assert((m == M{1,1,1,2,3,4,5,5,5,8,8,9,10}));
  }
  {
    using M = std::flat_multiset<int, std::greater<>, std::deque<int, min_allocator<int>>>;
    using It = cpp20_input_iterator<const int*>;
    M m = {8,5,3,2};
    int ar[] = {3,1,4,1,5,9,5,8};
    std::ranges::subrange r = { It(ar), sentinel_wrapper<It>(It(ar + 8)) };
    static_assert(!std::ranges::common_range<decltype(r)>);
    m.insert_range(r);
    assert((m == M{9,8,8,5,5,5,4,3,3,2,1,1}));
  }
  {
    // The "sorting" part uses the comparator, not operator==.
    struct ModTen {
      bool operator()(int a, int b) const { return (a % 10) < (b % 10); }
    };
    using M = std::flat_multiset<int, ModTen>;
    M m = {21, 43, 15, 37};
    int ar[] = {33, 18, 55, 18, 42};
    m.insert_range(ar);
    const int expected[] = {21, 42, 43, 33, 15, 55, 37, 18, 18};
    assert(std::ranges::is_permutation(m, expected));
    LIBCPP_ASSERT(std::ranges::equal(m, expected));
  }
  {
    // The "sorting" part uses the comparator, not operator==.
    struct ModTen {
      bool operator()(int a, int b) const { return (a % 10) < (b % 10); }
    };
    using M = std::flat_multiset<int, ModTen>;
    M m = {21, 43, 15, 37};
    int ar[] = {33, 18, 55, 28, 42};
    m.insert_range(ar);
    const int expected[] = {21, 42, 43, 33, 15, 55, 37, 18, 28};
    assert(std::ranges::is_permutation(m, expected));
    LIBCPP_ASSERT(std::ranges::equal(m, expected));
  }
  return 0;
}
