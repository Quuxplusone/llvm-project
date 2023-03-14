//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <deque>

// Test CTAD on cases where deduction should fail.

#include <deque>
#include <memory>

void test() {
  {
    // Cannot deduce T from nothing
    std::deque deq;
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'deque'}}
  }
  {
    // Cannot deduce T from just (Alloc)
    auto deq = std::deque(std::allocator<int>());
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'deque'}}
  }
  {
    // Cannot deduce T from just (Count)
    auto deq = std::deque(42);
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'deque'}}
  }
}
