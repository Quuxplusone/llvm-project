//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// flat_set& operator=(initializer_list<value_type> il);

#include <cassert>
#include <deque>
#include <flat_set>
#include <vector>

#include "test_macros.h"
#include "min_allocator.h"
#include "test_allocator.h"

int main(int, char**) {
  {
    using C = std::flat_set<int>;
    C m = {8, 10};
    assert(m.size() == 2);
    m = {3, 1, 2, 2, 3, 4, 3, 5, 6, 5};
    int expected[] = {1, 2, 3, 4, 5, 6};
    assert(std::equal(m.cbegin(), m.cend(), expected, expected + 6));
  }
  {
    using C = std::flat_set<int, std::less<>, std::vector<int, min_allocator<int>>>;
    C m = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    assert(m.size() == 10);
    m = {1, 3, 4, 5, 6, 5, 2};
    int expected[] = {1, 2, 3, 4, 5, 6};
    assert(std::equal(m.cbegin(), m.cend(), expected, expected + 6));
  }
  {
    using C = std::flat_set<double, std::less<>, std::deque<double, min_allocator<double>>>;
    C m = {};
    assert(m.size() == 0);
    m = {3, 1, 2, 2, 3, 4, 3, 5, 6, 5};
    double expected[] = {1, 2, 3, 4, 5, 6};
    assert(std::equal(m.cbegin(), m.cend(), expected, expected + 6));
  }
  {
    using C = std::flat_set<double, std::less<>, std::deque<double>>;
    C m = {10, 8};
    assert(m.size() == 2);
    m = {3};
    double expected[] = {3};
    assert(std::equal(m.cbegin(), m.cend(), expected, expected + 1));
  }

  return 0;
}
