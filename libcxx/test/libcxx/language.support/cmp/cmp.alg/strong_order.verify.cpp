//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// std::strong_order
//   Substitution failure should give reasonably nice diagnostics.

#include <compare>

void different_types() {
  std::strong_order(42, 42L);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'is_same_v<decay_t<int>, decay_t<long> >' evaluated to false}}
}

void no_threeway_operator() {
  struct S {} s;
  std::strong_order(s, s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'std::forward<_Tp>(__t) <=> std::forward<_Up>(__u)' would be invalid}}
  // expected-note@*:* {{and 'strong_order(std::forward<_Tp>(__t), std::forward<_Up>(__u))' would be invalid}}
}

void too_weak() {
  struct S {
    std::weak_ordering operator<=>(const S&) const = default;
  } s;
  std::strong_order(s, s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'strong_ordering(std::forward<_Tp>(__t) <=> std::forward<_Up>(__u))' would be invalid}}
  // expected-note@*:* {{and 'is_floating_point_v<decay_t<S &> >' evaluated to false}}
  // expected-note@*:* {{and 'strong_order(std::forward<_Tp>(__t), std::forward<_Up>(__u))' would be invalid}}
}
