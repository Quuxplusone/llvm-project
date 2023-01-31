//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

// void clear() noexcept;

#include <hive>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

void tests() {
  {
    int a[] = {1, 2, 3};
    std::hive<int> c(a, a+3);
    ASSERT_NOEXCEPT(c.clear());
    c.clear();
    assert(c.empty());
  }
  {
    int a[] = {1, 2, 3};
    std::hive<int, min_allocator<int>> c(a, a+3);
    ASSERT_NOEXCEPT(c.clear());
    c.clear();
    assert(c.empty());
  }
  {
    int a[] = {1, 2, 3};
    std::hive<int, safe_allocator<int>> c(a, a + 3);
    ASSERT_NOEXCEPT(c.clear());
    c.clear();
    assert(c.empty());
  }
}

int main(int, char**) {
  tests();
  return 0;
}
