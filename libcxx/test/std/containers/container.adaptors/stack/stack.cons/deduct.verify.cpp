//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <stack>

// Test CTAD on cases where deduction should fail.

#include <stack>
#include <list>
#include <memory>

void test() {
  {
    // Cannot deduce T from nothing
    std::stack s;
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'stack'}}
  }
  {
    // Cannot deduce T from just (Alloc)
    auto s = std::stack(std::allocator<int>());
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'stack'}}
  }
  {
    // The '45' is not an allocator
    auto s = std::stack(std::list<int>{1,2,3}, 45);
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'stack'}}
  }
  {
    // The '45' is not an allocator
    std::stack<int> source;
    auto s = std::stack(source, 45);
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'stack'}}
  }
}
