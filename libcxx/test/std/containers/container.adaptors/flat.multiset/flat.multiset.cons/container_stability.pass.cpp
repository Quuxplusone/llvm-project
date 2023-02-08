//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// explicit flat_multiset(container_type cont);
//
// libc++ uses stable_sort to ensure that flat_multiset's behavior matches multiset's,
// in terms of the order in which duplicate items are kept.
// This tests a conforming extension.

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <flat_set>
#include <random>
#include <set>
#include <vector>

#include "test_macros.h"

struct Mod256 {
  bool operator()(int x, int y) const { return (x % 256) < (y % 256); }
};

int main(int, char**)
{
  std::mt19937 randomness;
  std::vector<uint16_t> values;
  for (int i=0; i < 200; ++i) {
    uint16_t r = randomness();
    values.push_back(r);
  }

  {
    std::multiset<uint16_t, Mod256> s(values.begin(), values.end());
    std::flat_multiset<uint16_t, Mod256> fs(values);
    assert(fs.size() == s.size());
    LIBCPP_ASSERT(std::ranges::equal(fs, s));
  }
  {
    std::multiset<uint16_t, Mod256> s(values.begin(), values.end());
    std::flat_multiset<uint16_t, Mod256> fs(values, Mod256());
    assert(fs.size() == s.size());
    LIBCPP_ASSERT(std::ranges::equal(fs, s));
  }
  {
    std::multiset<uint16_t, Mod256> s(values.begin(), values.end());
    std::flat_multiset<uint16_t, Mod256> fs(values, std::allocator<int>());
    assert(fs.size() == s.size());
    LIBCPP_ASSERT(std::ranges::equal(fs, s));
  }
  {
    std::multiset<uint16_t, Mod256> s(values.begin(), values.end());
    std::flat_multiset<uint16_t, Mod256> fs(values, Mod256(), std::allocator<int>());
    assert(fs.size() == s.size());
    LIBCPP_ASSERT(std::ranges::equal(fs, s));
  }
  return 0;
}
