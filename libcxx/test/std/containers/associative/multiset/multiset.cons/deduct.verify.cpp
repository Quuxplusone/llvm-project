//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <set>

// Test CTAD on cases where deduction should fail.

#include <set>
#include <functional>
#include <memory>

struct NotAnAllocator {
  friend bool operator<(NotAnAllocator, NotAnAllocator) { return false; }
};

void test() {
  {
    // cannot deduce Key from nothing
    std::multiset s;
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'multiset'}}
  }
  {
    // cannot deduce Key from just (Compare)
    std::multiset s(std::less<int>{});
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'multiset'}}
  }
  {
    // cannot deduce Key from just (Compare, Allocator)
    std::multiset s(std::less<int>{}, std::allocator<int>{});
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'multiset'}}
  }
  {
    // cannot deduce Key from multiset(Allocator)
    std::multiset s(std::allocator<int>{});
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'multiset'}}
  }
  {
    // since we have parens, not braces, this deliberately does not find the
    // initializer_list constructor
    NotAnAllocator a;
    std::multiset s(a);
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'multiset'}}
  }
}
