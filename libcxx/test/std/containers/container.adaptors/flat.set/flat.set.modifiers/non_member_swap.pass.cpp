//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// void swap(flat_set&, flat_set&);

#include <cassert>
#include <deque>
#include <flat_set>
#include <functional>
#include <vector>

#include "min_allocator.h"
#include "test_allocator.h"
#include "test_macros.h"
#include "../../../test_compare.h"

int main(int, char**)
{
  {
    using M = std::flat_set<int>;
    M m1;
    M m2 = {1,2,3,4};
    M m1_save = m1;
    M m2_save = m2;
    swap(m1, m2); // calls hidden-friend swap
    assert(m1 == m2_save);
    assert(m2 == m1_save);
    m1.swap(m2);
    assert(m1 == m1_save);
    assert(m2 == m2_save);
  }
  {
    using M = std::flat_set<int, std::greater<>, std::deque<int, min_allocator<int>>>;
    M m1 = {3,5,6};
    M m2 = {1,2,3,4};
    M m1_save = m1;
    M m2_save = m2;
    std::swap(m1, m2); // works, but uses the slow three-step algorithm
    assert(m1 == m2_save);
    assert(m2 == m1_save);
    m1.swap(m2);
    assert(m1 == m1_save);
    assert(m2 == m2_save);
  }
  {
    // Comparators are also swapped
    using A = test_allocator<int>;
    using C = test_less<int>;
    using M = std::flat_set<int, C, std::vector<int, A>>;
    M m1 = M({1,5,9}, C(1), A(1, 1));
    M m2 = M({1,2,3,4}, C(2), A(1, 2));
    M m1_save = m1;
    M m2_save = m2;
    using std::swap;
    swap(m1, m2); // calls hidden-friend swap
    assert(m1 == m2_save);
    assert(m2 == m1_save);
    assert(m1.key_comp() == C(2));
    assert(m2.key_comp() == C(1));
  }
  return 0;
}
