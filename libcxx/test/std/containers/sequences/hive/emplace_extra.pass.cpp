//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

// template <class... Args> iterator emplace_hint(const_iterator pos, Args&&... args);

#include <hive>
#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "test_allocator.h"

void tests() {
  {
    std::hive<int> v;
    v.reserve(3);
    v = { 1, 2, 3 };
    v.emplace_hint(v.begin(), v.back());
    int expected[] = {1, 2, 3, 3};
    assert(std::is_permutation(v.begin(), v.end(), expected, expected + 4));
  }
  {
    std::hive<int> v;
    v.reserve(4);
    v = { 1, 2, 3 };
    v.emplace_hint(v.begin(), v.back());
    int expected[] = {1, 2, 3, 3};
    assert(std::is_permutation(v.begin(), v.end(), expected, expected + 4));
  }
  {
    std::hive<int, min_allocator<int>> v;
    v.reserve(3);
    v = { 1, 2, 3 };
    v.emplace_hint(v.begin(), v.back());
    int expected[] = {1, 2, 3, 3};
    assert(std::is_permutation(v.begin(), v.end(), expected, expected + 4));
  }
  {
    std::hive<int, min_allocator<int>> v;
    v.reserve(4);
    v = { 1, 2, 3 };
    v.emplace_hint(v.begin(), v.back());
    int expected[] = {1, 2, 3, 3};
    assert(std::is_permutation(v.begin(), v.end(), expected, expected + 4));
  }
  {
    std::hive<int> v;
    v.reserve(8);
    size_t old_capacity = v.capacity();
    assert(old_capacity >= 8);

    v.resize(4); // keep the existing capacity
    assert(v.capacity() == old_capacity);

    v.push_back(42);
    assert(v.size() == 5);
    assert(v.capacity() == old_capacity);
    int expected[] = {0,0,0,0,42};
    assert(std::is_permutation(v.begin(), v.end(), expected, expected + 5));
  }
}

int main(int, char**) {
  tests();
  return 0;
}
