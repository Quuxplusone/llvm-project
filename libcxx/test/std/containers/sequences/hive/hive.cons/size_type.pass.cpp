//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

// explicit hive(size_type n);

#include <hive>
#include <cassert>
#include <cstddef>
#include "test_macros.h"
#include "DefaultOnly.h"
#include "test_allocator.h"
#include "min_allocator.h"

template <class T, class Allocator>
void
test3(unsigned n, Allocator const &alloc = Allocator())
{
#if TEST_STD_VER > 11
  typedef std::hive<T, Allocator> C;
  {
  C d(n, alloc);
  assert(d.size() == n);
  assert(static_cast<std::size_t>(std::distance(d.begin(), d.end())) == n);
  assert(d.get_allocator() == alloc);
  }
#else
  ((void)n);
  ((void)alloc);
#endif
}


int main(int, char**)
{
  {
    std::hive<int> l(3);
    assert(l.size() == 3);
    assert(std::distance(l.begin(), l.end()) == 3);
    std::hive<int>::const_iterator i = l.begin();
    assert(*i == 0);
    ++i;
    assert(*i == 0);
    ++i;
    assert(*i == 0);
  }
  {
    // Add 2 for implementations that dynamically allocate a sentinel node and container proxy.
    std::hive<int, limited_allocator<int, 5>> l(3);
    assert(l.size() == 3);
    assert(std::distance(l.begin(), l.end()) == 3);
    std::hive<int, limited_allocator<int, 5>>::const_iterator i = l.begin();
    assert(*i == 0);
    ++i;
    assert(*i == 0);
    ++i;
    assert(*i == 0);
  }
  {
    typedef std::hive<int, min_allocator<int> > C;
    C l(3, min_allocator<int> ());
    assert(l.size() == 3);
    assert(std::distance(l.begin(), l.end()) == 3);
    C::const_iterator i = l.begin();
    assert(*i == 0);
    ++i;
    assert(*i == 0);
    ++i;
    assert(*i == 0);
    test3<int, min_allocator<int>> (3);
  }
  {
    std::hive<DefaultOnly> l(3);
    assert(l.size() == 3);
    assert(std::distance(l.begin(), l.end()) == 3);
  }
  {
    std::hive<int, min_allocator<int>> l(3);
    assert(l.size() == 3);
    assert(std::distance(l.begin(), l.end()) == 3);
    std::hive<int, min_allocator<int>>::const_iterator i = l.begin();
    assert(*i == 0);
    ++i;
    assert(*i == 0);
    ++i;
    assert(*i == 0);
  }
  {
    std::hive<DefaultOnly, min_allocator<DefaultOnly>> l(3);
    assert(l.size() == 3);
    assert(std::distance(l.begin(), l.end()) == 3);
  }
  return 0;
}
