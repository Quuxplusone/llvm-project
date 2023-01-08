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
  // expected-note@*:* {{because 'remove_reference_t<int>' (aka 'int') does not satisfy '__class_or_enum'}}
  // expected-note@*:* {{because '_Bool(is_class_v<remove_reference_t<int> > || is_union_v<remove_reference_t<int> > || is_enum_v<remove_reference_t<int> >)' evaluated to false}}
  // expected-note@*:* {{and 'ranges::begin(__t)' would be invalid}}
}

void g() {
  struct S {} s;
  std::ranges::data(s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because '__t.data()' would be invalid}}
  // expected-note@*:* {{and 'ranges::begin(__t)' would be invalid}}
}
