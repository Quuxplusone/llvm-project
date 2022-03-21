//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// std::ranges::data
//   Substitution failure should give reasonably nice diagnostics.

#include <ranges>

void f() {
  std::ranges::data(42);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'int' does not satisfy '__member_data'}}
  // expected-note@*:* {{because 'int' does not satisfy '__can_borrow'}}
  // expected-note@*:* {{because 'is_lvalue_reference_v<int>' evaluated to false}}
  // expected-note@*:* {{and 'enable_borrowed_range<remove_cvref_t<int> >' evaluated to false}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'int' does not satisfy '__ranges_begin_invocable'}}
  // expected-note@*:* {{because 'int' does not satisfy '__can_borrow'}}
  // expected-note@*:* {{because 'is_lvalue_reference_v<int>' evaluated to false}}
  // expected-note@*:* {{and 'enable_borrowed_range<remove_cvref_t<int> >' evaluated to false}}
}

void g() {
  struct S {} s;
  std::ranges::data(s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'S &' does not satisfy '__member_data'}}
  // expected-note@*:* {{because 'static_cast<typename decay<decltype((__t.data()))>::type>(__t.data())' would be invalid}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'S &' does not satisfy '__ranges_begin_invocable'}}
  // expected-note@*:* {{because 'ranges::begin(__t)' would be invalid}}
}
