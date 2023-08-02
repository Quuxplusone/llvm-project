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
    std::set s;
      // expected-error-re@-1{{no viable constructor or deduction guide for deduction of template arguments of '{{(std::)?}}set'}}
  }
  {
    // cannot deduce Key from just (Compare)
    std::set s(std::less<int>{});
      // expected-error-re@-1{{no viable constructor or deduction guide for deduction of template arguments of '{{(std::)?}}set'}}
  }
  {
    // cannot deduce Key from just (Compare, Allocator)
    std::set s(std::less<int>{}, std::allocator<int>{});
      // expected-error-re@-1{{no viable constructor or deduction guide for deduction of template arguments of '{{(std::)?}}set'}}
  }
  {
    // cannot deduce Key from just (Allocator)
    std::set s(std::allocator<int>{});
      // expected-error-re@-1{{no viable constructor or deduction guide for deduction of template arguments of '{{(std::)?}}set'}}
  }
  {
    // since we have parens, not braces, this deliberately does not find the
    // initializer_list constructor
    NotAnAllocator a;
    std::set s(a);
      // expected-error-re@-1{{no viable constructor or deduction guide for deduction of template arguments of '{{(std::)?}}set'}}
  }
}
