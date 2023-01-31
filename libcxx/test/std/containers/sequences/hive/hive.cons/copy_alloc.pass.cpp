//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

// hive(const hive& c, const allocator_type& a);

#include <hive>
#include <algorithm>
#include <cassert>
#include "test_macros.h"
#include "DefaultOnly.h"
#include "test_allocator.h"
#include "min_allocator.h"

int main(int, char**)
{
  {
    std::hive<int, test_allocator<int> > l(3, 2, test_allocator<int>(5));
    std::hive<int, test_allocator<int> > l2(l, test_allocator<int>(3));
    assert(std::equal(l2.begin(), l2.end(), l.begin(), l.end()));
    assert(l2.get_allocator() == test_allocator<int>(3));
  }
  {
    std::hive<int, other_allocator<int> > l(3, 2, other_allocator<int>(5));
    std::hive<int, other_allocator<int> > l2(l, other_allocator<int>(3));
    assert(std::equal(l2.begin(), l2.end(), l.begin(), l.end()));
    assert(l2.get_allocator() == other_allocator<int>(3));
  }
  {
    std::hive<int, min_allocator<int> > l(3, 2, min_allocator<int>());
    std::hive<int, min_allocator<int> > l2(l, min_allocator<int>());
    assert(std::equal(l2.begin(), l2.end(), l.begin(), l.end()));
    assert(l2.get_allocator() == min_allocator<int>());
  }
  return 0;
}
