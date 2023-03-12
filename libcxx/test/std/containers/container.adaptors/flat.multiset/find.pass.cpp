//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

//       iterator find(const key_type& k);
// const_iterator find(const key_type& k) const;

#include <cassert>
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
    M m = {1, 2, 4, 5, 5, 5, 8};
    ASSERT_SAME_TYPE(decltype(m.find(0)), M::iterator);
    ASSERT_SAME_TYPE(decltype(std::as_const(m).find(0)), M::const_iterator);
    assert(m.find(0) == m.end());
    assert(m.find(1) == m.begin());
    assert(m.find(2) == m.begin() + 1);
    assert(m.find(3) == m.end());
    assert(m.find(4) == m.begin() + 2);
    assert(m.find(5) != m.end() && *m.find(5) == 5);
    LIBCPP_ASSERT(m.find(5) == m.begin() + 3); // return the earliest match
    assert(m.find(6) == m.end());
    assert(m.find(7) == m.end());
    assert(std::as_const(m).find(8) == m.begin() + 6);
    assert(std::as_const(m).find(9) == m.end());
  }
  {
    using M = std::flat_multiset<int, std::greater<int>, std::deque<int, min_allocator<int>>>;
    M m = {1, 2, 4, 5, 5, 5, 8};
    ASSERT_SAME_TYPE(decltype(m.find(0)), M::iterator);
    ASSERT_SAME_TYPE(decltype(std::as_const(m).find(0)), M::const_iterator);
    assert(m.find(0) == m.end());
    assert(m.find(1) == m.begin() + 6);
    assert(m.find(2) == m.begin() + 5);
    assert(m.find(3) == m.end());
    assert(m.find(4) == m.begin() + 4);
    assert(m.find(5) != m.end() && *m.find(5) == 5);
    LIBCPP_ASSERT(m.find(5) == m.begin() + 1); // return the earliest match
    assert(m.find(6) == m.end());
    assert(m.find(7) == m.end());
    assert(std::as_const(m).find(8) == m.begin());
    assert(std::as_const(m).find(9) == m.end());
  }
  {
    using M = std::flat_multiset<bool>;
    M m = {true, false};
    ASSERT_SAME_TYPE(decltype(m.find(0)), M::iterator);
    ASSERT_SAME_TYPE(decltype(std::as_const(m).find(0)), M::const_iterator);
    assert(m.find(true) == m.begin() + 1);
    assert(m.find(false) == m.begin());
    m = {true};
    assert(m.find(true) == m.begin());
    assert(m.find(false) == m.end());
    m = {false};
    assert(std::as_const(m).find(true) == m.end());
    assert(std::as_const(m).find(false) == m.begin());
    m.clear();
    assert(m.find(true) == m.end());
    assert(m.find(false) == m.end());
  }
  return 0;
}
