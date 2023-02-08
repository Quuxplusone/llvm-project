//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// template<container-compatible-range<value_type> R>
//   void insert_range(R&& rg);
//
// libc++ uses stable_sort to ensure that flat_multiset's behavior matches multiset's,
// in terms of which duplicate items are kept.
// This tests a conforming extension.

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <flat_set>
#include <random>
#include <ranges>
#include <set>

#include "test_macros.h"

struct Mod256 {
  bool operator()(int x, int y) const { return (x % 256) < (y % 256); }
};

int main(int, char**)
{
  {
    std::mt19937 randomness;
    uint16_t values[400];
    std::generate(values, values + 400, randomness);

    std::multiset<uint16_t, Mod256> s(values, values + 200);
    std::flat_multiset<uint16_t, Mod256> fs(std::sorted_equivalent, s.begin(), s.end());
    assert(std::ranges::equal(fs, s));

    fs.insert_range(std::views::counted(values + 200, 200));
    s.insert(values + 200, values + 400);
    assert(fs.size() == s.size());
    LIBCPP_ASSERT(std::ranges::equal(fs, s));
  }
  return 0;
}
