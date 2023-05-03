//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_map>

// size_type count(const key_type& x) const;

#include <cassert>
#include <deque>
#include <flat_map>
#include <functional>
#include <utility>

#include "test_macros.h"
#include "min_allocator.h"

int main(int, char**)
{
  {
    using M = std::flat_multimap<int, const char*>;
    M m = {{1,""}, {2,""}, {4,""}, {5,""}, {5,"a"}, {5,""}, {8,""}, {8,"a"}};
    ASSERT_SAME_TYPE(decltype(m.count(0)), size_t);
    assert(m.count(0) == 0);
    assert(m.count(1) == 1);
    assert(m.count(2) == 1);
    assert(m.count(3) == 0);
    assert(m.count(4) == 1);
    assert(m.count(5) == 3);
    assert(m.count(6) == 0);
    assert(m.count(7) == 0);
    assert(std::as_const(m).count(8) == 2);
    assert(std::as_const(m).count(9) == 0);
  }
  {
    using M = std::flat_multimap<int, int, std::greater<int>, std::deque<int, min_allocator<int>>>;
    M m = {{1,0}, {2,0}, {4,0}, {5,0}, {5,1}, {5,2}, {8,0}, {8,1}};
    ASSERT_SAME_TYPE(decltype(m.count(0)), size_t);
    assert(m.count(0) == 0);
    assert(m.count(1) == 1);
    assert(m.count(2) == 1);
    assert(m.count(3) == 0);
    assert(m.count(4) == 1);
    assert(m.count(5) == 3);
    assert(m.count(6) == 0);
    assert(m.count(7) == 0);
    assert(std::as_const(m).count(8) == 2);
    assert(std::as_const(m).count(9) == 0);
  }
  {
    using M = std::flat_multimap<bool, int>;
    M m = {{true,1}, {false,2}, {false,3}};
    ASSERT_SAME_TYPE(decltype(m.count(0)), size_t);
    assert(m.count(true) == 1);
    assert(m.count(false) == 2);
    m = {{true,3},{true,3}};
    assert(m.count(true) == 2);
    assert(m.count(false) == 0);
    m = {{false,4},{false,3}};
    assert(std::as_const(m).count(true) == 0);
    assert(std::as_const(m).count(false) == 2);
    m.clear();
    assert(m.count(true) == 0);
    assert(m.count(false) == 0);
  }
  return 0;
}
