//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// flat_multiset();

#include <cassert>
#include <deque>
#include <flat_set>
#include <vector>

#include "test_macros.h"
#include "min_allocator.h"

struct DefaultCtableComp {
  explicit DefaultCtableComp() { default_constructed_ = true; }
  bool operator()(int, int) const { return false; }
  bool default_constructed_ = false;
};

int main(int, char**)
{
  {
    std::flat_multiset<int> m;
    assert(m.empty());
  }
  {
    std::flat_multiset<int> m = {};
    assert(m.empty());
  }
  {
    std::flat_multiset<int, DefaultCtableComp, std::deque<int, min_allocator<int>>> m;
    assert(m.empty());
    assert(m.begin() == m.end());
    assert(m.key_comp().default_constructed_);
  }
  {
    using A = explicit_allocator<int>;
    {
      std::flat_multiset<int, DefaultCtableComp, std::vector<int, A>> m;
      assert(m.empty());
      assert(m.key_comp().default_constructed_);
    }
    {
      A a;
      std::flat_multiset<int, DefaultCtableComp, std::vector<int, A>> m(a);
      assert(m.empty());
      assert(m.key_comp().default_constructed_);
    }
  }
  return 0;
}
