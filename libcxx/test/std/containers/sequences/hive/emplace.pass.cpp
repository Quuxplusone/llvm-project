//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

// template <class... Args>
//   iterator emplace(Args&&... args);

#include <algorithm>
#include <cassert>
#include <hive>
#include <utility>

#include "test_macros.h"
#include "../../Emplaceable.h"
#include "min_allocator.h"

int main(int, char**)
{
  {
    using M = std::hive<Emplaceable>;
    M m;
    ASSERT_SAME_TYPE(decltype(m.emplace()), M::iterator);
    M::iterator r = m.emplace();
    assert(r == m.begin());
    assert(*r == Emplaceable());
    assert(m.size() == 1);
    r = m.emplace(2, 3.5);
    assert(*r == Emplaceable(2, 3.5));
    assert(m.size() == 2);
    r = m.emplace(2, 3.5);
    assert(*r == Emplaceable(2, 3.5));
    assert(m.size() == 3);
  }
  {
    using M = std::hive<int>;
    int expected[] = {1, 2, 3, 4, 5, 2, 0};
    M m = {1, 3, 4};
    ASSERT_SAME_TYPE(decltype(m.emplace(2)), M::iterator);
    M::iterator r = m.emplace(2);
    assert(*r == 2);
    assert(std::is_permutation(m.begin(), m.end(), expected, expected + 4));
    r = m.emplace(5);
    assert(*r == 5);
    assert(std::is_permutation(m.begin(), m.end(), expected, expected + 5));
    r = m.emplace(2);
    assert(*r == 2);
    assert(std::is_permutation(m.begin(), m.end(), expected, expected + 6));
    r = m.emplace(); // value-initialize int(0)
    assert(*r == 0);
    assert(std::is_permutation(m.begin(), m.end(), expected, expected + 7));
  }
  {
    using M = std::hive<int, min_allocator<int>>;
    int expected[] = {1, 2, 3, 4, 5, 2, 0};
    M m = {1, 3, 4};
    ASSERT_SAME_TYPE(decltype(m.emplace(2)), M::iterator);
    M::iterator r = m.emplace(2);
    assert(*r == 2);
    assert(std::is_permutation(m.begin(), m.end(), expected, expected + 4));
    r = m.emplace(5);
    assert(*r == 5);
    assert(std::is_permutation(m.begin(), m.end(), expected, expected + 5));
    r = m.emplace(2);
    assert(*r == 2);
    assert(std::is_permutation(m.begin(), m.end(), expected, expected + 6));
    r = m.emplace(); // value-initialize int(0)
    assert(*r == 0);
    assert(std::is_permutation(m.begin(), m.end(), expected, expected + 7));
  }
  return 0;
}
