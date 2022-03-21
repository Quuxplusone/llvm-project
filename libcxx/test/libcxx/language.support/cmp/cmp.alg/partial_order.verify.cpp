//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// std::partial_order
//   Substitution failure should give reasonably nice diagnostics.

#include <compare>

void different_types() {
  std::partial_order(42, 42L);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
}

void no_threeway_operator() {
  struct S {} s;
  std::partial_order(s, s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
}

void too_weak() {
  struct S {
    int operator<=>(const S&) const { return 0; }
  } s;
  std::partial_order(s, s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
}
