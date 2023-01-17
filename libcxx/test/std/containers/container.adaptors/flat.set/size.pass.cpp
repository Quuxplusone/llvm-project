//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// size_type size() const;

#include <cassert>
#include <flat_set>

int main(int, char**)
{
  {
    using M = std::flat_set<int>;
    M m;
    assert(m.size() == 0);
    m.insert(2);
    assert(m.size() == 1);
    m.insert({3,5,2,4}); // 2 is a duplicate
    assert(m.size() == 4); // {2,3,4,5}
    m.erase(m.begin());
    assert(m.size() == 3);
    m.erase(m.begin() + 1);
    assert(m.size() == 2);
    m.clear();
    assert(m.size() == 0);
  }
  return 0;
}
