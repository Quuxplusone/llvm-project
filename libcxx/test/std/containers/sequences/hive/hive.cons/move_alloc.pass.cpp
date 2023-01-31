//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

// hive(hive&& c, const allocator_type& a);

#include <hive>
#include <algorithm>
#include <cassert>
#include "test_macros.h"
#include "MoveOnly.h"
#include "test_allocator.h"
#include "min_allocator.h"

int main(int, char**)
{
  {
    std::hive<MoveOnly, test_allocator<MoveOnly> > l(test_allocator<MoveOnly>(5));
    std::hive<MoveOnly, test_allocator<MoveOnly> > lo(test_allocator<MoveOnly>(5));
    for (int i = 1; i <= 3; ++i)
    {
      l.push_back(i);
      lo.push_back(i);
    }
    std::hive<MoveOnly, test_allocator<MoveOnly> > l2(std::move(l), test_allocator<MoveOnly>(6));
    assert(std::equal(l2.begin(), l2.end(), lo.begin(), lo.end()));
    assert(!l.empty());
    assert(l2.get_allocator() == test_allocator<MoveOnly>(6));
  }
  {
    std::hive<MoveOnly, test_allocator<MoveOnly> > l(test_allocator<MoveOnly>(5));
    std::hive<MoveOnly, test_allocator<MoveOnly> > lo(test_allocator<MoveOnly>(5));
    for (int i = 1; i <= 3; ++i)
    {
      l.push_back(i);
      lo.push_back(i);
    }
    std::hive<MoveOnly, test_allocator<MoveOnly> > l2(std::move(l), test_allocator<MoveOnly>(5));
    assert(std::equal(l2.begin(), l2.end(), lo.begin(), lo.end()));
    assert(l.empty());
    assert(l2.get_allocator() == test_allocator<MoveOnly>(5));
  }
  {
    std::hive<MoveOnly, other_allocator<MoveOnly> > l(other_allocator<MoveOnly>(5));
    std::hive<MoveOnly, other_allocator<MoveOnly> > lo(other_allocator<MoveOnly>(5));
    for (int i = 1; i <= 3; ++i)
    {
      l.push_back(i);
      lo.push_back(i);
    }
    std::hive<MoveOnly, other_allocator<MoveOnly> > l2(std::move(l), other_allocator<MoveOnly>(4));
    assert(std::equal(l2.begin(), l2.end(), lo.begin(), lo.end()));
    assert(!l.empty());
    assert(l2.get_allocator() == other_allocator<MoveOnly>(4));
  }
  {
    std::hive<MoveOnly, min_allocator<MoveOnly> > l(min_allocator<MoveOnly>{});
    std::hive<MoveOnly, min_allocator<MoveOnly> > lo(min_allocator<MoveOnly>{});
    for (int i = 1; i <= 3; ++i)
    {
      l.push_back(i);
      lo.push_back(i);
    }
    std::hive<MoveOnly, min_allocator<MoveOnly> > l2(std::move(l), min_allocator<MoveOnly>());
    assert(std::equal(l2.begin(), l2.end(), lo.begin(), lo.end()));
    assert(l.empty());
    assert(l2.get_allocator() == min_allocator<MoveOnly>());
  }
  return 0;
}
