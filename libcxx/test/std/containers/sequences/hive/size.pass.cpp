//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

// size_type size() const noexcept;

#include <hive>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

int main(int, char**)
{
  {
    std::hive<int> c;
    ASSERT_NOEXCEPT(c.size());
    assert(c.size() == 0);
    c.insert(2);
    assert(c.size() == 1);
    c.insert(1);
    assert(c.size() == 2);
    c.insert(3);
    assert(c.size() == 3);
    c.erase(c.begin());
    assert(c.size() == 2);
    c.erase(c.begin());
    assert(c.size() == 1);
    c.erase(c.begin());
    assert(c.size() == 0);
  }
  {
    std::hive<int, min_allocator<int>> c;
    ASSERT_NOEXCEPT(c.size());
    assert(c.size() == 0);
    c.insert(2);
    assert(c.size() == 1);
    c.insert(1);
    assert(c.size() == 2);
    c.insert(3);
    assert(c.size() == 3);
    c.erase(c.begin());
    assert(c.size() == 2);
    c.erase(c.begin());
    assert(c.size() == 1);
    c.erase(c.begin());
    assert(c.size() == 0);
  }
  return 0;
}
