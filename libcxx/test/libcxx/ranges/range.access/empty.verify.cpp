//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// std::ranges::empty
//   Substitution failure should give reasonably nice diagnostics.

#include <ranges>

void f() {
  std::ranges::empty(42);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'int' does not satisfy '__member_empty'}}
  // expected-note@*:* {{because 'int' does not satisfy '__workaround_52970'}}
  // expected-note@*:* {{because 'is_class_v<__remove_cvref_t<int> >' evaluated to false}}
  // expected-note@*:* {{and 'is_union_v<__remove_cvref_t<int> >' evaluated to false}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'int' does not satisfy '__can_invoke_size'}}
  // expected-note@*:* {{because 'ranges::size(__t)' would be invalid}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'int' does not satisfy '__can_compare_begin_end'}}
  // expected-note@*:* {{because 'bool(ranges::begin(__t) == ranges::end(__t))' would be invalid}}
}

void g() {
  struct S {} s;
  std::ranges::empty(s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'S &' does not satisfy '__member_empty'}}
  // expected-note@*:* {{because 'bool(__t.empty())' would be invalid}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'S &' does not satisfy '__can_invoke_size'}}
  // expected-note@*:* {{because 'ranges::size(__t)' would be invalid}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'S &' does not satisfy '__can_compare_begin_end'}}
  // expected-note@*:* {{because 'bool(ranges::begin(__t) == ranges::end(__t))' would be invalid}}
}
