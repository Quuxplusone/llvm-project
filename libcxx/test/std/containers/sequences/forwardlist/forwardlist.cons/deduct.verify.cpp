//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <forward_list>

// Test CTAD on cases where deduction should fail.

#include <forward_list>
#include <memory>

struct A {};

void test() {
  {
    // Cannot deduce T from nothing
    std::forward_list fwl;
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'forward_list'}}
  }
  {
    // Cannot deduce T from just (Alloc)
    auto fwl = std::forward_list(std::allocator<int>());
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'forward_list'}}
  }
  {
    // Cannot deduce T from just (Count)
    auto fwl = std::forward_list(42);
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'forward_list'}}
  }
}
