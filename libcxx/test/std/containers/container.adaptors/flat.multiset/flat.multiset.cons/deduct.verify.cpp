//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// Test CTAD on cases where deduction should fail.

#include <flat_set>
#include <functional>
#include <memory>

struct NotAnAllocator {
  friend bool operator<(NotAnAllocator, NotAnAllocator) { return false; }
};

void test() {
  {
    // cannot deduce Key from nothing
    std::flat_multiset s;
    // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'flat_multiset'}}
  }
  {
    // cannot deduce Key from just (Compare)
    std::flat_multiset s(std::less<int>{});
    // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'flat_multiset'}}
  }
  {
    // cannot deduce Key from just (Compare, Allocator)
    std::flat_multiset s(std::less<int>{}, std::allocator<int>{});
    // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'flat_multiset'}}
  }
  {
    // cannot deduce Key from just (Allocator)
    std::flat_multiset s(std::allocator<int>{});
    // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'flat_multiset'}}
  }
  {
    // since we have parens, not braces, this deliberately does not find the
    // initializer_list constructor
    NotAnAllocator a;
    std::flat_multiset s(a);
    // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'flat_multiset'}}
  }
}
