//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// template<class InputIterator>
//   flat_multiset(InputIterator first, InputIterator last, const key_compare& comp = key_compare())
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

#include "test_macros.h"

struct Mod256 {
  bool operator()(int x, int y) const { return (x % 256) < (y % 256); }
};

int main(int, char**)
{
  std::mt19937 randomness;
  uint16_t values[200];
  std::generate(values, values + 200, randomness);

  {
    std::multiset<uint16_t, Mod256> s(values, values + 200);
    std::flat_multiset<uint16_t, Mod256> fs(values, values + 200);
    assert(fs.size() == s.size());
    LIBCPP_ASSERT(std::ranges::equal(fs, s));
  }
  {
    std::multiset<uint16_t, Mod256> s(values, values + 200, std::allocator<int>());
    std::flat_multiset<uint16_t, Mod256> fs(values, values + 200, std::allocator<int>());
    assert(fs.size() == s.size());
    LIBCPP_ASSERT(std::ranges::equal(fs, s));
  }
  {
    std::multiset<uint16_t, Mod256> s(values, values + 200, Mod256());
    std::flat_multiset<uint16_t, Mod256> fs(values, values + 200, Mod256());
    assert(fs.size() == s.size());
    LIBCPP_ASSERT(std::ranges::equal(fs, s));
  }
  {
    std::multiset<uint16_t, Mod256> s(values, values + 200, Mod256(), std::allocator<int>());
    std::flat_multiset<uint16_t, Mod256> fs(values, values + 200, Mod256(), std::allocator<int>());
    assert(fs.size() == s.size());
    LIBCPP_ASSERT(std::ranges::equal(fs, s));
  }
  return 0;
}
