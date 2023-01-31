//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

// void resize(size_t n);

#include <hive>
#include <algorithm>
#include <cassert>
#include <vector>
#include <cstdio>

#include "test_macros.h"
#include "min_allocator.h"

#ifndef TEST_HAS_NO_EXCEPTIONS
struct ThrowingCtor {
  explicit ThrowingCtor() { throw 42; }
  explicit ThrowingCtor(int) { }
};
#endif // TEST_HAS_NO_EXCEPTIONS

int main(int, char**)
{
  {
    std::hive<int> c = {1};
    std::vector<int> v = {1};
    ASSERT_SAME_TYPE(decltype(c.resize(1)), void);
    assert(c.size() == 1 && c.capacity() >= c.size());
    size_t oldcap = c.capacity();
    c.resize(oldcap); // should not allocate
    v.resize(oldcap);
    assert(c.size() == oldcap);
    assert(c.capacity() == oldcap);
    assert(std::is_permutation(c.begin(), c.end(), v.begin(), v.end()));
    c.insert(2);
    v.push_back(2);
    assert(std::is_permutation(c.begin(), c.end(), v.begin(), v.end()));
    oldcap = c.capacity();
    c.resize(oldcap); // should not allocate
    v.resize(oldcap);
    assert(c.size() == oldcap);
    assert(c.capacity() == oldcap);
    assert(std::is_permutation(c.begin(), c.end(), v.begin(), v.end()));
fprintf(stderr, "%d %d\n", (int)c.size(), (int)c.capacity());
    c.resize(10);
fprintf(stderr, "%d %d\n", (int)c.size(), (int)c.capacity());
    assert(c.size() == 10);
    c.resize(5);
    assert(c.size() == 5);
    c.resize(0);
    assert(c.empty());
  }
  {
    std::hive<int, min_allocator<int>> c = {1};
    std::vector<int> v = {1};
    ASSERT_SAME_TYPE(decltype(c.resize(1)), void);
    assert(c.size() == 1 && c.capacity() >= c.size());
    size_t oldcap = c.capacity();
    c.resize(oldcap); // should not allocate
    v.resize(oldcap);
    assert(c.size() == oldcap);
    assert(c.capacity() == oldcap);
    assert(std::is_permutation(c.begin(), c.end(), v.begin(), v.end()));
    c.insert(2);
    v.push_back(2);
    assert(std::is_permutation(c.begin(), c.end(), v.begin(), v.end()));
    oldcap = c.capacity();
    c.resize(oldcap); // should not allocate
    v.resize(oldcap);
    assert(c.size() == oldcap);
    assert(c.capacity() == oldcap);
    assert(std::is_permutation(c.begin(), c.end(), v.begin(), v.end()));
    c.resize(10);
    assert(c.size() == 10);
    c.resize(5);
    assert(c.size() == 5);
    c.resize(0);
    assert(c.empty());
  }
#ifndef TEST_HAS_NO_EXCEPTIONS
  {
    std::hive<ThrowingCtor> c;
    c.emplace(1);
    c.emplace(2);
    c.resize(1); // shrink
    try {
      c.resize(2);
      assert(false);
    } catch (int ex) {
      assert(ex == 42);
      assert(c.size() == 1);
    }
  }
#endif // TEST_HAS_NO_EXCEPTIONS
  return 0;
}
