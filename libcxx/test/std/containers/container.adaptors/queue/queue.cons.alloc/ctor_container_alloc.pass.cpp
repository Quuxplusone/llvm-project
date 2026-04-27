//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <queue>

// template <class Alloc>
//   queue(const container_type& c, const Alloc& a);

#include <queue>
#include <cassert>
#include <cstddef>
#include <list>

#include "test_macros.h"
#include "test_allocator.h"

template <class C>
TEST_CONSTEXPR_CXX26 C make(int n) {
  C c;
  for (int i = 0; i < n; ++i)
    c.push_back(i);
  return c;
}

typedef std::list<int, test_allocator<int> > C;

struct test : public std::queue<int, C> {
  typedef std::queue<int, C> base;

  TEST_CONSTEXPR_CXX26 explicit test(const test_allocator<int>& a) : base(a) {}
  TEST_CONSTEXPR_CXX26 test(const container_type& container, const test_allocator<int>& a) : base(container, a) {}
#if TEST_STD_VER >= 11
  TEST_CONSTEXPR_CXX26 test(container_type&& container, const test_allocator<int>& a) : base(std::move(container), a) {}
  TEST_CONSTEXPR_CXX26 test(test&& q, const test_allocator<int>& a) : base(std::move(q), a) {}
#endif
  TEST_CONSTEXPR_CXX26 test_allocator<int> get_allocator() { return c.get_allocator(); }
};

TEST_CONSTEXPR_CXX26 bool run_test() {
  C d = make<C>(5);
  test q(d, test_allocator<int>(4));
  assert(q.get_allocator() == test_allocator<int>(4));
  assert(q.size() == 5);
  for (C::size_type i = 0; i < d.size(); ++i) {
    assert(q.front() == int(i));
    q.pop();
  }

  return true;
}

int main(int, char**) {
  run_test();
#if TEST_STD_VER >= 26
  static_assert(run_test());
#endif

  return 0;
}
