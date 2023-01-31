//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

// hive(size_type n, const T& value, const Allocator& = Allocator());

#include <hive>
#include <cassert>
#include "test_macros.h"
#include "min_allocator.h"

int main(int, char**)
{
  {
    std::hive<int> l(3, 2);
    assert(l.size() == 3);
    assert(std::distance(l.begin(), l.end()) == 3);
    std::hive<int>::const_iterator i = l.begin();
    assert(*i == 2);
    ++i;
    assert(*i == 2);
    ++i;
    assert(*i == 2);
  }
  {
    std::hive<int> l(3, 2, std::allocator<int>());
    assert(l.size() == 3);
    assert(std::distance(l.begin(), l.end()) == 3);
    std::hive<int>::const_iterator i = l.begin();
    assert(*i == 2);
    ++i;
    assert(*i == 2);
    ++i;
    assert(*i == 2);
  }
  {
    std::hive<int, min_allocator<int>> l(3, 2);
    assert(l.size() == 3);
    assert(std::distance(l.begin(), l.end()) == 3);
    std::hive<int, min_allocator<int>>::const_iterator i = l.begin();
    assert(*i == 2);
    ++i;
    assert(*i == 2);
    ++i;
    assert(*i == 2);
  }
  {
    std::hive<int, min_allocator<int>> l(3, 2, min_allocator<int>());
    assert(l.size() == 3);
    assert(std::distance(l.begin(), l.end()) == 3);
    std::hive<int, min_allocator<int>>::const_iterator i = l.begin();
    assert(*i == 2);
    ++i;
    assert(*i == 2);
    ++i;
    assert(*i == 2);
  }
  return 0;
}
