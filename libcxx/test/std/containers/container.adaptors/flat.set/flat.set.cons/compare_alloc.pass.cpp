//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// template <class Alloc>
//   flat_set(const key_compare& comp, const Alloc& a);

#include <cassert>
#include <deque>
#include <flat_set>
#include <vector>

#include "test_macros.h"
#include "../../../test_compare.h"
#include "test_allocator.h"

int main(int, char**)
{
  {
    using C = test_less<int>;
    using A = test_allocator<int>;
    auto m = std::flat_set<int, C, std::vector<int, A>>(C(4), A(5));
    assert(m.empty());
    assert(m.begin() == m.end());
    assert(m.key_comp() == C(4));
    assert(std::move(m).extract().get_allocator() == A(5));
  }
  {
    using C = test_less<int>;
    using A = test_allocator<int>;
    std::flat_set<int, C, std::deque<int, A>> m = { C(4), A(5) }; // implicit ctor
    assert(m.empty());
    assert(m.begin() == m.end());
    assert(m.key_comp() == C(4));
    assert(std::move(m).extract().get_allocator() == A(5));
  }
  return 0;
}
