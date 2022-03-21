//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// std::compare_partial_order_fallback
//   Substitution failure should give reasonably nice diagnostics.

#include <compare>
#include <utility>

void different_types() {
  std::compare_partial_order_fallback(42, 42L);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'is_same_v<decay_t<int>, decay_t<long> >' evaluated to false}}
}

void no_operators() {
  struct S {} s;
  std::compare_partial_order_fallback(s, s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'std::partial_order(std::forward<_Tp>(__t), std::forward<_Up>(__u))' would be invalid}}
  // expected-note@*:* {{and 'std::forward<_Tp>(__t) == std::forward<_Up>(__u)' would be invalid}}
}

void not_enough_operators() {
  struct S {
    bool operator==(const S&) const { return false; }
    bool operator<(const S&) { return false; } // deliberately asymmetric
  } s;
  std::compare_partial_order_fallback(s, std::as_const(s));
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'std::partial_order(std::forward<_Tp>(__t), std::forward<_Up>(__u))' would be invalid}}
  // expected-note@*:* {{and 'std::forward<_Up>(__u) < std::forward<_Tp>(__t)' would be invalid}}
}
