//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <queue>

// void displace_top();

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"

int main(int, char**)
{
  {
    std::priority_queue<int> q;
    ASSERT_SAME_TYPE(decltype(q.displace_top()), int);
    q.push(1);
    assert(q.displace_top() == 1);
    assert(q.empty());
    q.push(1);
    q.push(3);
    q.push(2);
    assert(q.displace_top() == 3);
    assert(q.displace_top() == 2);
    assert(q.displace_top() == 1);
    assert(q.empty());
  }
#if TEST_STD_VER >= 11
  {
    std::priority_queue<MoveOnly> q;
    ASSERT_SAME_TYPE(decltype(q.displace_top()), MoveOnly);
    q.push(1);
    assert(q.displace_top() == MoveOnly(1));
    assert(q.empty());
    q.push(1);
    q.push(3);
    q.push(2);
    assert(q.displace_top() == MoveOnly(3));
    assert(q.displace_top() == MoveOnly(2));
    assert(q.displace_top() == MoveOnly(1));
    assert(q.empty());
  }
#endif
  return 0;
}
