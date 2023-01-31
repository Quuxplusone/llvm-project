//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

// template <class... Args> reference emplace_back(Args&&... args);

#include <hive>
#include <cassert>
#include "test_macros.h"
#include "min_allocator.h"
#include "test_allocator.h"

class A
{
  int i_;
  double d_;

public:
  A(const A&) = delete;
  A& operator=(const A&) = delete;

  explicit A(int i, double d)
    : i_(i), d_(d) {}

  A(A&& a)
    : i_(a.i_),
      d_(a.d_) {
    a.i_ = 0;
    a.d_ = 0;
  }

  A& operator=(A&& a) {
    i_ = a.i_;
    d_ = a.d_;
    a.i_ = 0;
    a.d_ = 0;
    return *this;
  }

  int geti() const {return i_;}
  double getd() const {return d_;}
};

void tests()
{
  {
    std::hive<A> c;
    ASSERT_SAME_TYPE(decltype(c.emplace_back(2, 3.5)), A&);
    A& r1 = c.emplace_back(2, 3.5);
    assert(c.size() == 1);
    assert(&r1 == &c.back());
    assert(c.front().geti() == 2);
    assert(c.front().getd() == 3.5);
    A& r2 = c.emplace_back(3, 4.5);
    assert(c.size() == 2);
    assert(&r2 == &c.back());
    assert(c.front().geti() == 2);
    assert(c.front().getd() == 3.5);
    assert(c.back().geti() == 3);
    assert(c.back().getd() == 4.5);
  }
  {
    std::hive<A, min_allocator<A>> c;
    ASSERT_SAME_TYPE(decltype(c.emplace_back(2, 3.5)), A&);
    A& r1 = c.emplace_back(2, 3.5);
    assert(c.size() == 1);
    assert(&r1 == &c.back());
    assert(c.front().geti() == 2);
    assert(c.front().getd() == 3.5);
    A& r2 = c.emplace_back(3, 4.5);
    assert(c.size() == 2);
    assert(&r2 == &c.back());
    assert(c.front().geti() == 2);
    assert(c.front().getd() == 3.5);
    assert(c.back().geti() == 3);
    assert(c.back().getd() == 4.5);
  }
  {
    std::hive<Tag_X, TaggingAllocator<Tag_X>> c;
    c.emplace_back();
    assert(c.size() == 1);
    c.emplace_back(1, 2, 3);
    assert(c.size() == 2);
  }
  {
    // LWG 2164
    int arr[] = {0, 1, 2, 3, 4};
    int sz = 5;
    std::hive<int> c(arr, arr+sz);
    ASSERT_SAME_TYPE(decltype(c.emplace_back(2, 3.5)), int&);
    while (c.size() < c.capacity())
      c.push_back(sz++);
    c.emplace_back(c.front());
    assert(c.back() == 0);
    auto it = c.begin();
    for (int i = 0; i < sz; ++i) {
      assert(*it == i);
      ++it;
    }
    assert(*it == 0);
    ++it;
    assert(it == c.end());
  }
}

int main(int, char**)
{
  tests();
  return 0;
}
