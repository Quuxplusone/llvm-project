//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// void insert(sorted_unique_t, initializer_list<value_type> il);

#include <deque>
#include <flat_set>
#include <functional>
#include <vector>

#include "test_macros.h"
#include "min_allocator.h"
#include "MoveOnly.h"

int main(int, char**)
{
  {
    using C = std::flat_set<int>;
    C m = {0, 1, 7, 8};
    m.insert(std::sorted_unique, {1, 3, 5, 8, 9});
    assert((m == C{0, 1, 3, 5, 7, 8, 9}));
  }
  {
    using C = std::flat_set<int, std::greater<int>, std::deque<int, min_allocator<int>>>;
    C m = {0, 1, 7, 8};
    m.insert(std::sorted_unique, {9, 8, 5, 3, 1});
    assert((m == C{0, 1, 3, 5, 7, 8, 9}));
  }
  return 0;
}
