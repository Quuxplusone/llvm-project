//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// std::compare_strong_order_fallback
//   Substitution failure should give reasonably nice diagnostics.

#include <compare>

void different_types() {
  std::compare_strong_order_fallback(42, 42L);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
}

void no_operators() {
  struct S {} s;
  std::compare_strong_order_fallback(s, s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
}

void not_enough_operators() {
  struct S {
    bool operator==(const S&) const { return false; }
  } s;
  std::compare_strong_order_fallback(s, s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
}
