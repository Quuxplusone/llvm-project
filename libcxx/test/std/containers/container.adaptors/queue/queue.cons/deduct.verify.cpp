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
#include <list>
#include <memory>

void test()
{
  {
    // Cannot deduce T from nothing
    std::queue q;
      // expected-error-re@-1{{no viable constructor or deduction guide for deduction of template arguments of '{{(std::)?}}queue'}}
  }
  {
    // Cannot deduce T from just (Alloc)
    auto q = std::queue(std::allocator<int>());
      // expected-error-re@-1{{no viable constructor or deduction guide for deduction of template arguments of '{{(std::)?}}queue'}}
  }
  {
    // The '45' is not an allocator
    auto q = std::queue(std::list<int>{1,2,3}, 45);
      // expected-error-re@-1{{no viable constructor or deduction guide for deduction of template arguments of '{{(std::)?}}queue'}}
  }
  {
    // The '45' is not an allocator
    std::queue<int> source;
    auto q = std::queue(source, 45);
      // expected-error-re@-1{{no viable constructor or deduction guide for deduction of template arguments of '{{(std::)?}}queue'}}
  }
}
