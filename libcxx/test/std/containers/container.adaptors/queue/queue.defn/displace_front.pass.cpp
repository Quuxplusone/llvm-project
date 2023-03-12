//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <queue>

// void displace_top();

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"

TEST_CONSTEXPR_CXX26 bool test()
{
  {
    std::queue<int> q;
    ASSERT_SAME_TYPE(decltype(q.displace_front()), int);
    q.push(1);
    assert(q.displace_front() == 1);
    assert(q.empty());
    q.push(1);
    q.push(3);
    q.push(2);
    assert(q.displace_front() == 1);
    assert(q.displace_front() == 3);
    assert(q.displace_front() == 2);
    assert(q.empty());
  }
#if TEST_STD_VER >= 11
  {
    std::queue<MoveOnly> q;
    ASSERT_SAME_TYPE(decltype(q.displace_front()), MoveOnly);
    q.push(1);
    assert(q.displace_front() == MoveOnly(1));
    assert(q.empty());
    q.push(1);
    q.push(3);
    q.push(2);
    assert(q.displace_front() == MoveOnly(1));
    assert(q.displace_front() == MoveOnly(3));
    assert(q.displace_front() == MoveOnly(2));
    assert(q.empty());
  }
#endif
  return true;
}

int main(int, char**)
{
  test();
#if 0 && TEST_STD_VER >= 26 // to be enabled when constexpr deque arrives
  static_assert(test());
#endif

  return 0;
}
