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
#include <type_traits>

struct NotAnAllocator {
  friend bool operator<(NotAnAllocator, NotAnAllocator) { return false; }
};

int main(int, char **) {
  {
    // cannot deduce Key from nothing
    std::flat_set s;
    // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'flat_set'}}
  }
  {
    // cannot deduce Key from just (Compare)
    std::flat_set s(std::less<int>{});
    // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'flat_set'}}
  }
  {
    // cannot deduce Key from just (Compare, Allocator)
    std::flat_set s(std::less<int>{}, std::allocator<int>{});
    // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'flat_set'}}
  }
  {
    // cannot deduce Key from just (Allocator)
    std::flat_set s(std::allocator<int>{});
    // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'flat_set'}}
  }
  {
    // since we have parens, not braces, this deliberately does not find the
    // initializer_list constructor
    NotAnAllocator a;
    std::flat_set s(a);
    // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'flat_set'}}
  }
  return 0;
}
