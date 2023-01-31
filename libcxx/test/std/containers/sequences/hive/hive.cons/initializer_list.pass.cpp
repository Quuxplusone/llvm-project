//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

// hive(initializer_hive<value_type> il, const Allocator& a = allocator_type());

#include <hive>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "test_allocator.h"

int main(int, char**)
{
  {
    std::hive<int> d = {3, 4, 5, 6};
    assert(d.size() == 4);
    std::hive<int>::iterator i = d.begin();
    assert(*i++ == 3);
    assert(*i++ == 4);
    assert(*i++ == 5);
    assert(*i++ == 6);
  }
  {
    std::hive<int, test_allocator<int>> d({3, 4, 5, 6}, test_allocator<int>(3));
    assert(d.get_allocator() == test_allocator<int>(3));
    assert(d.size() == 4);
    std::hive<int, test_allocator<int>>::iterator i = d.begin();
    assert(*i++ == 3);
    assert(*i++ == 4);
    assert(*i++ == 5);
    assert(*i++ == 6);
  }
  {
    std::hive<int, min_allocator<int>> d = {3, 4, 5, 6};
    assert(d.size() == 4);
    std::hive<int, min_allocator<int>>::iterator i = d.begin();
    assert(*i++ == 3);
    assert(*i++ == 4);
    assert(*i++ == 5);
    assert(*i++ == 6);
  }
  {
    std::hive<int, min_allocator<int>> d = {{3, 4, 5, 6}, min_allocator<int>()};
    assert(d.get_allocator() == min_allocator<int>());
    assert(d.size() == 4);
    std::hive<int, min_allocator<int>>::iterator i = d.begin();
    assert(*i++ == 3);
    assert(*i++ == 4);
    assert(*i++ == 5);
    assert(*i++ == 6);
  }
  return 0;
}
