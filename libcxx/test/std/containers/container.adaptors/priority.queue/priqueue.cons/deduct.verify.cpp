//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <queue>

// Test CTAD on cases where deduction should fail.

#include <queue>
#include <deque>
#include <functional>
#include <memory>

void test()
{
  {
    // Cannot deduce T from nothing
    std::priority_queue pri;
      // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'priority_queue'}}
  }
  {
    // Cannot deduce T from just (Alloc)
    auto pri = std::priority_queue(std::allocator<int>());
      // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'priority_queue'}}
  }
  {
    // The '45' is not an allocator
    auto pri = std::priority_queue(std::greater<int>(), std::deque<int>({1,2,3}), 45);
      // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'priority_queue'}}
  }
  {
    // The '45' is not an allocator
    std::priority_queue<int> source;
    auto pri = std::priority_queue(source, 45);
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'priority_queue'}}
  }
  {
    // int is not an iterator
    auto pri = std::priority_queue(15, 17, std::greater<double>());
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'priority_queue'}}
  }
  {
    // float is not an iterator
    auto pri = std::priority_queue(23.f, 2.f, std::greater<float>(), std::deque<float>());
       // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'priority_queue'}}
  }
}
